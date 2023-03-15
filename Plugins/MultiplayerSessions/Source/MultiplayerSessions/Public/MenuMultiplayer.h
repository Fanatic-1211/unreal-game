// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuMultiplayer.generated.h"

class UButton;
class UMultiplayerSessionSubsystem;

/*
 * Class with logic for multiplayer visual menu.
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenuMultiplayer : public UUserWidget
{
    GENERATED_BODY()

public:
    // Adds widget to viewport and binds callbacks to delegates
    UFUNCTION(BlueprintCallable)
    void MenuSetup(int32 NumberOfPublicConnections = 4,
        FString TypeOfMatch = TEXT("FreeForAllCepk"),
        FString LobbyPath = TEXT("/Game/Levels/MultiplayerLobbyLevel"));

protected:
    virtual void NativeOnInitialized() override;  // begin play analog for widgets
    virtual void OnLevelRemovedFromWorld(
        ULevel* InLevel, UWorld* InWorld) override;  // default UE callback

    // Callbacks for custom delegates
    void OnFindSessions(
        const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
    void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
    UFUNCTION()
    void OnCreateSession(bool bWasSuccessful);
    UFUNCTION()
    void OnStartSession(bool bWasSuccessful);
    UFUNCTION()
    void OnDestroySession(bool bWasSuccessful);

private:
    UMultiplayerSessionSubsystem* MultiplayerSubsystem;  // custom class with main multiplayer logic

    int32 NumPublicConnections = 4;  // max players in one session
    FString MatchType = TEXT("FreeForAllCepk");
    FString PathToLobbyLevel = TEXT("");

    UPROPERTY(meta = (BindWidget))
    UButton* HostButton;
    UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;

    UFUNCTION()
    void HostButtonClicked();
    UFUNCTION()
    void JoinButtonClicked();

    // Removes menu widget and reset input to default value
    void MenuTearDown();
};
