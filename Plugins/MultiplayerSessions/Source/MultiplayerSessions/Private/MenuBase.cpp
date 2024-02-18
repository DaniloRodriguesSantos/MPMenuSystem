// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuBase.h"
#include "MultiplayerSessionsSubsystem.h"
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

void UMenuBase::MenuSetup()
{
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
}

void UMenuBase::HostButtonClicked()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("Host Button Clicked."))
		);
	}

	if(multiplayerSessionsSubsystem)
	{
		multiplayerSessionsSubsystem->CreateSession(4, FString("FreeForAll"));
	}
}

void UMenuBase::JoinButtonClicked()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("Join Button Clicked."))
		);
	}
}
