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
    void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = TEXT("FreeForAllCepk"));

protected:
    virtual void NativeOnInitialized() override;
    virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:
    UMultiplayerSessionSubsystem* MultiplayerSubsystem;

    int32 NumPublicConnections = 4;
    FString MatchType = TEXT("FreeForAllCepk");

    UPROPERTY(meta = (BindWidget))
    UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;

	UFUNCTION()
    void HostButtonClicked();
	UFUNCTION()
    void JoinButtonClicked();

    void MenuTearDown();
};
