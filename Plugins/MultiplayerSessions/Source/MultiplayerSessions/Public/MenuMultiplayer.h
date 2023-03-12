// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuMultiplayer.generated.h"

class UButton;
class UMultiplayerSessionSubsystem;

UCLASS()
class MULTIPLAYERSESSIONS_API UMenuMultiplayer : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void MenuSetup();

protected:
    virtual void NativeOnInitialized() override;

private:
    UMultiplayerSessionSubsystem* MultiplayerSubsystem;

    UPROPERTY(meta = (BindWidget))
    UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;

	UFUNCTION()
    void HostButtonClicked();
	UFUNCTION()
    void JoinButtonClicked();
};


