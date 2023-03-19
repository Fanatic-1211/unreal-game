// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerSessionSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

DEFINE_LOG_CATEGORY_STATIC(Log_MultiplayerSessionSubsystem, All, All);

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

    UE_LOG(Log_MultiplayerSessionSubsystem, Verbose,
        TEXT("MultiplayerSessionSubsystem Constructor worked successfully."));
}

void UMultiplayerSessionSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
    if (!SessionInterface.IsValid()) return;

    const FName OSSName = IOnlineSubsystem::Get()->GetSubsystemName();
    FString OSSS = OSSName.ToString();
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, (TEXT("%s"), OSSS));

    auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
    if (ExistingSession != nullptr) SessionInterface->DestroySession(NAME_GameSession);

    // Store delegate in handle for later remove it from the delegate list:
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
    LastSessionSettings->BuildUniqueId = 78;  // different game build versions can't see each other
    LastSessionSettings->Set(FName("MatchType"), MatchType,
        EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);  // set match type

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    if (!SessionInterface->CreateSession(
            *LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
    {
        UE_LOG(Log_MultiplayerSessionSubsystem, Error,
            TEXT("CreateSession: Session creation failed!"));

        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegateHandle);

        // Broadcast to custom delegate:
        MultiplayerOnCreateSessionComplete.Broadcast(false);
    }

    UE_LOG(
        Log_MultiplayerSessionSubsystem, Log, TEXT("CreateSession: Session created successfully."));
}

void UMultiplayerSessionSubsystem::FindSessions(int32 MaxSearchResults)
{
    if (!SessionInterface.IsValid()) return;

    FindSessionsCompleteDelegateHandle =
        SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

    LastSessionSearch = MakeShareable(new FOnlineSessionSearch);
    LastSessionSearch->MaxSearchResults = MaxSearchResults;
    LastSessionSearch->bIsLanQuery =
        IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
    LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    if (!SessionInterface->FindSessions(
            *LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
    {
        UE_LOG(Log_MultiplayerSessionSubsystem, Error, TEXT("FindSessions: Session search error!"));

        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(
            FindSessionsCompleteDelegateHandle);
        MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
    }

    UE_LOG(Log_MultiplayerSessionSubsystem, Log, TEXT("FindSessions: found %d active sessions."),
        LastSessionSearch->SearchResults.Num());
}

void UMultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
    if (!SessionInterface.IsValid())
    {
        UE_LOG(Log_MultiplayerSessionSubsystem, Error,
            TEXT("JoinSession: SessionInterface has not found!"));

        MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
        return;
    }

    JoinSessionCompleteDelegateHandle =
        SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    if (!SessionInterface->JoinSession(
            *LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
    {
        UE_LOG(Log_MultiplayerSessionSubsystem, Error, TEXT("JoinSession: Session join error!"));

        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegateHandle);
        MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
    }

    UE_LOG(
        Log_MultiplayerSessionSubsystem, Log, TEXT("JoinSession: Successfully join to session."));
}

void UMultiplayerSessionSubsystem::DestroySession() {}

void UMultiplayerSessionSubsystem::StartSession() {}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (SessionInterface)
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegateHandle);

    // Broadcast custom delegate:
    MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);

    UE_LOG(Log_MultiplayerSessionSubsystem, Verbose,
        TEXT("OnCreateSessionComplete: Delegate has broadcasted."));
}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (SessionInterface)
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(
            FindSessionsCompleteDelegateHandle);

    if (LastSessionSearch->SearchResults.Num() <= 0)
    {
        UE_LOG(Log_MultiplayerSessionSubsystem, Error,
            TEXT("OnFindSessionsComplete: Delegate has broadcasted FAILURE: found 0 sessions!"));

        MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
        return;
    }

    MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);

    UE_LOG(Log_MultiplayerSessionSubsystem, Verbose,
        TEXT("OnFindSessionsComplete: Delegate has broadcasted."));
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(
    FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface)
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegateHandle);

    MultiplayerOnJoinSessionComplete.Broadcast(Result);

    UE_LOG(Log_MultiplayerSessionSubsystem, Verbose,
        TEXT("OnJoinSessionComplete: Delegate has broadcasted."));
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful) {}