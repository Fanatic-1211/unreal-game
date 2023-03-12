// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuMultiplayer.generated.h"

class UButton;
class UMultiplayerSessionSubsystem;

UCLASS()
class MULTIPLAYERSESSIONS_API UMenuMultiplayer : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void MenuSetup(
        int32 NumberOfPublicConnections = 4, FString TypeOfMatch = TEXT("FreeForAllCepk"));

protected:
    virtual void NativeOnInitialized() override;
    virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

    // Callbacks for custom delegates
    UFUNCTION()
    void OnCreateSession(bool bWasSuccessful);
    UFUNCTION()
    void OnStartSession(bool bWasSuccessful);
    UFUNCTION()
    void OnDestroySession(bool bWasSuccessful);

    void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
    void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

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
