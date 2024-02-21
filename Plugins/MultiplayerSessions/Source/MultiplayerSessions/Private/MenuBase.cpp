// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuBase.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"

bool UMenuBase::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	if(ButtonHost)
	{
		ButtonHost->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}

	if(ButtonJoin)
	{
		ButtonJoin->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	
	return true;
}

void UMenuBase::NativeDestruct()
{
	MenuTeardDown();
	Super::NativeDestruct();
}

void UMenuBase::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("Session crated successfully."))
			);
		}

		UWorld* world = GetWorld();
		if(world)
		{
			world->ServerTravel("/Game/ThirdPerson/Maps/Lobby?listen");
		}
	}
	else
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
	}
}

void UMenuBase::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if(multiplayerSessionsSubsystem != nullptr)
	{
		return;
	}
	
	for (FOnlineSessionSearchResult result : SessionResults)
	{
		FString SettingsValue;
		result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if(SettingsValue == matchType)
		{
			multiplayerSessionsSubsystem->JoinSession(result);
			return;
		}
	}
}

void UMenuBase::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	if(multiplayerSessionsSubsystem)
	{
		if(multiplayerSessionsSubsystem->GetSessionInterface().IsValid())
		{
			FString address;
			multiplayerSessionsSubsystem->GetSessionInterface()->GetResolvedConnectString(NAME_GameSession, address);

			APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();
			if(playerController)
			{
				playerController->ClientTravel(address, TRAVEL_Absolute);
			}
		}
	}
}

void UMenuBase::OnDestroySession(bool bWasSuccessful)
{
}

void UMenuBase::OnStartSession(bool bWasSuccessful)
{
}

void UMenuBase::MenuSetup(int32 InNumPublicConnections, FString InMatchType)
{
	numPublicConnections = InNumPublicConnections;
	matchType = InMatchType;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	UWorld* world = GetWorld();
	if(world)
	{
		APlayerController* playerController = world->GetFirstPlayerController();
		if(playerController)
		{
			FInputModeUIOnly inputModeData;
			inputModeData.SetWidgetToFocus(TakeWidget());
			inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			playerController->SetInputMode(inputModeData);
			playerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* gameInstance = GetGameInstance();
	if(gameInstance)
	{
		multiplayerSessionsSubsystem = gameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (multiplayerSessionsSubsystem)
	{
		multiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		multiplayerSessionsSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::OnFindSession);
		multiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		multiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		multiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

void UMenuBase::HostButtonClicked()
{
	if(multiplayerSessionsSubsystem)
	{
		multiplayerSessionsSubsystem->CreateSession(numPublicConnections, matchType);
	}
}

void UMenuBase::JoinButtonClicked()
{
	if(multiplayerSessionsSubsystem)
	{
		multiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UMenuBase::MenuTeardDown()
{
	RemoveFromParent();
	UWorld* world = GetWorld();
	if(world)
	{
		APlayerController* playerController = world->GetFirstPlayerController();
		if(playerController)
		{
			FInputModeGameOnly inputModeData;
			playerController->SetInputMode(inputModeData);
			playerController->SetShowMouseCursor(false);
		}
	}
}
