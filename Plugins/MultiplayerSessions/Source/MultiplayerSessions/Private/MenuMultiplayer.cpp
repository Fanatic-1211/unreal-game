// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuMultiplayer.h"

#include "Components/Button.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "MultiplayerSessionSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(Log_MenuMultiplayer, All, All);

void UMenuMultiplayer::NativeOnInitialized()
{
    UE_LOG(Log_MenuMultiplayer, Verbose, TEXT("NativeOnInitialized: started..."));

    Super::NativeOnInitialized();

    if (HostButton) HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
    if (JoinButton) JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);

    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        MultiplayerSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
        UE_LOG(Log_MenuMultiplayer, Verbose, TEXT("NativeOnInitialized: Successfully finished."));
    }
}

// Calls when level changes
void UMenuMultiplayer::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
    MenuTearDown();
    Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenuMultiplayer::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch)
{
    NumPublicConnections = NumberOfPublicConnections;
    MatchType = TypeOfMatch;

    // Widget and player input setup:
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;

    if (GetWorld())
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

        if (PlayerController)
        {
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(true);
        }
    }

    // Binding callbacks to delegates:
    if (MultiplayerSubsystem)
    {
        MultiplayerSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(
            this, &ThisClass::OnCreateSession);
        MultiplayerSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(
            this, &ThisClass::OnDestroySession);
        MultiplayerSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(
            this, &ThisClass::OnStartSession);
        MultiplayerSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(
            this, &ThisClass::OnFindSessions);
        MultiplayerSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(
            this, &ThisClass::OnJoinSession);

        UE_LOG(Log_MenuMultiplayer, VeryVerbose,
            TEXT("MenuSetup: All delegates have binded successfully."));
    }
}

void UMenuMultiplayer::MenuTearDown()
{
    RemoveFromParent();

    if (GetWorld())
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            FInputModeGameOnly InputModeData;
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(false);
        }
    }
}

void UMenuMultiplayer::HostButtonClicked()
{
    UE_LOG(Log_MenuMultiplayer, Display, TEXT("Host Button clicked."));

    if (MultiplayerSubsystem) MultiplayerSubsystem->CreateSession(NumPublicConnections, MatchType);
}

void UMenuMultiplayer::JoinButtonClicked()
{
    UE_LOG(Log_MenuMultiplayer, Display, TEXT("Join Button clicked."));

    if (MultiplayerSubsystem) MultiplayerSubsystem->FindSessions(10000);
}

// ---------------------------------------
// Callbacks definitions
// ---------------------------------------
void UMenuMultiplayer::OnCreateSession(bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(Log_MenuMultiplayer, Log,
            TEXT("Session created succsessfully! Starting travel to lobby level..."));

        if (GetWorld())
            GetWorld()->ServerTravel(
                "/Game/Levels/MultiplayerLobbyLevel?listen");  // TODO: remove hardcode
    }
    else
    {
        UE_LOG(Log_MenuMultiplayer, Error, TEXT("OnCreateSession: Session creation failed!"));
    }
}

void UMenuMultiplayer::OnStartSession(bool bWasSuccessful) {}

void UMenuMultiplayer::OnDestroySession(bool bWasSuccessful) {}

void UMenuMultiplayer::OnFindSessions(
    const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
    UE_LOG(Log_MenuMultiplayer, Log, TEXT("OnFindSessions: Searching proper session..."));

    for (auto Result : SessionResults)
    {
        if (MultiplayerSubsystem == nullptr) return;

        FString SettingsValue;
        FString Id = Result.GetSessionIdStr();
        FString User = Result.Session.OwningUserName;
        Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
        UE_LOG(
            Log_MenuMultiplayer, VeryVerbose, TEXT("Sessions search: Id %s, User %s"), *Id, *User);

        if (SettingsValue == MatchType)
        {
            UE_LOG(Log_MenuMultiplayer, Log, TEXT("Proper session found: %s, %s"), *Id, *User);
            MultiplayerSubsystem->JoinSession(Result);
            return;
        }
    }

    UE_LOG(Log_MenuMultiplayer, Warning, TEXT("OnFindSessions: Proper session doesn't found!"));
    return;
}

void UMenuMultiplayer::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
    UE_LOG(Log_MenuMultiplayer, Log, TEXT("OnJoinSession: Trying to connect to host..."));

    IOnlineSubsystem* OnlineSystem = IOnlineSubsystem::Get();

    if (OnlineSystem)
    {
        IOnlineSessionPtr SessionInterface = OnlineSystem->GetSessionInterface();

        if (SessionInterface.IsValid())
        {
            FString Address;
            SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

            APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
            if (PlayerController)
            {
                PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
            }
        }
    }
}