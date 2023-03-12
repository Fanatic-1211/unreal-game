// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerSessionSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem()
    : CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(
          this, &ThisClass::OnCreateSessionComplete)),
      FindSessionsCompleteDelegate(
          FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
      JoinSessionCompleteDelegate(
          FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
      DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(
          this, &ThisClass::OnDestroySessionComplete)),
      StartSessionCompleteDelegate(
          FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
    IOnlineSubsystem* OnlineSystem = IOnlineSubsystem::Get();

    if (OnlineSystem) SessionInterface = OnlineSystem->GetSessionInterface();
}

void UMultiplayerSessionSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
    if (!SessionInterface.IsValid()) return;

    auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
    if (ExistingSession != nullptr) SessionInterface->DestroySession(NAME_GameSession);

    // Store delegate in handle for later remove it from the delegate list
    CreateSessionCompleteDelegateHandle =
        SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    LastSessionSettings = MakeShareable(new FOnlineSessionSettings());

    LastSessionSettings->bIsLANMatch =
        IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
    LastSessionSettings->NumPublicConnections = NumPublicConnections;
    LastSessionSettings->bAllowJoinInProgress = true;
    LastSessionSettings->bAllowJoinViaPresence = true;  // check world region for join game
    LastSessionSettings->bUsesPresence = true;          // use player world region for search game
    LastSessionSettings->bShouldAdvertise = true;       // allow find your session in public
    LastSessionSettings->bUseLobbiesIfAvailable = true;
    LastSessionSettings->Set(FName("MatchType"), MatchType,
        EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);  // set match type

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    if (!SessionInterface->CreateSession(
            *LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegateHandle);
    }
}

void UMultiplayerSessionSubsystem::FindSessions(int32 MaxSearchResults) {}

void UMultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult) {}

void UMultiplayerSessionSubsystem::DestroySession() {}

void UMultiplayerSessionSubsystem::StartSession() {}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful) {}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(
    FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful) {}