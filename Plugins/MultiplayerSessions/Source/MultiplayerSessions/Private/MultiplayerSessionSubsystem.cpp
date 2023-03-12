// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"

#include "OnlineSubsystem.h"

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem() :
      CreateSessionCompleteDelegate(
          FOnCreateSessionCompleteDelegate::CreateUObject(
              this, &ThisClass::OnCreateSessionComplete)),
      FindSessionsCompleteDelegate(
          FOnFindSessionsCompleteDelegate::CreateUObject(
              this, &ThisClass::OnFindSessionsComplete)),
      JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(
          this, &ThisClass::OnJoinSessionComplete)),
      DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(
          this, &ThisClass::OnDestroySessionComplete)),
      StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(
          this, &ThisClass::OnStartSessionComplete))
{
    IOnlineSubsystem* OnlineSystem = IOnlineSubsystem::Get();

    if (OnlineSystem)
    {
        SessionInterface = OnlineSystem->GetSessionInterface();
    }
}

void UMultiplayerSessionSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	
}

void UMultiplayerSessionSubsystem::FindSessions(int32 MaxSearchResults)
{
	
}

void UMultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	
}

void UMultiplayerSessionSubsystem::DestroySession()
{
	
}

void UMultiplayerSessionSubsystem::StartSession()
{
	
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	
}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	
}