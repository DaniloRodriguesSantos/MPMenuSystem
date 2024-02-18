// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonHost;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonJoin;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* multiplayerSessionsSubsystem;
};
