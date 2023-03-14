// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuMultiplayer.h"

#include "Components/Button.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "MultiplayerSessionSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuMultiplayer, All, All);

void UMenuMultiplayer::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (HostButton) HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
    if (JoinButton) JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);

    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
        MultiplayerSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
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
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString(TEXT("HostButton clicked!")));
    UE_LOG(LogMenuMultiplayer, Display, TEXT("Host Button clicked."));
    UE_LOG(LogTemp, Error, TEXT("Host Button clicked."));

    if (MultiplayerSubsystem) MultiplayerSubsystem->CreateSession(NumPublicConnections, MatchType);
}

void UMenuMultiplayer::JoinButtonClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString(TEXT("JoinButton clicked!")));
    UE_LOG(LogMenuMultiplayer, Display, TEXT("Join Button clicked."));

    if (MultiplayerSubsystem) MultiplayerSubsystem->FindSessions(10000);
}

// ---------------------------------------
// Callbacks definitions
// ---------------------------------------
void UMenuMultiplayer::OnCreateSession(bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 3.f, FColor::Green, FString(TEXT("Session created succsessfully!")));

        if (GetWorld())
            GetWorld()->ServerTravel(
                "/Game/Levels/MultiplayerLobbyLevel?listen");  // TODO: remove hardcode
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 3.f, FColor::Red, FString(TEXT("Session creation failed!")));
    }
}

void UMenuMultiplayer::OnStartSession(bool bWasSuccessful) {}

void UMenuMultiplayer::OnDestroySession(bool bWasSuccessful) {}

void UMenuMultiplayer::OnFindSessions(
    const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
    for (auto Result : SessionResults)
    {
        if (MultiplayerSubsystem == nullptr) return;

        FString SettingsValue;
        Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);

        if (SettingsValue == MatchType)
        {
            MultiplayerSubsystem->JoinSession(Result);
            return;
        }
    }
}

void UMenuMultiplayer::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
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