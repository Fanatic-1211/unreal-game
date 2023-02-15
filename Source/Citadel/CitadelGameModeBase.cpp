// Fill out your copyright notice in the Description page of Project Settings.


#include "CitadelGameModeBase.h"

#include "AIController.h"

#include "Components/PlayerStateBase.h"
#include "Players/PlayerGround.h"


ACitadelGameModeBase::ACitadelGameModeBase()
{
    PlayerStateClass = APlayerStateBase::StaticClass();
}


void ACitadelGameModeBase::StartPlay()
{
    Super::StartPlay();

    StartNewRound();
    SpawnBots();
    CreateTeamsInfo();
    
}

UClass* ACitadelGameModeBase::GetDefaultPawnClassForController_Implementation
    (AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ACitadelGameModeBase::StartNewRound()
{
    RoundCountdown = GameData.RoundDuration;

    GetWorldTimerManager().SetTimer(
        RoundTimerHandle, this, &ACitadelGameModeBase::UpdateRoundTimer, 1.f, true);

    UE_LOG(LogTemp, Warning, TEXT("New Round Started!"));

}

void ACitadelGameModeBase::UpdateRoundTimer()
{
    if (--RoundCountdown == 0)
    {
        GetWorldTimerManager().ClearTimer(RoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            StartNewRound();
            ResetPlayers();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Game Over!"));
        }
    }
}

void ACitadelGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;

    for (int32 i = 0; i < GameData.PlayersNum; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = 
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AAIController* Controller = GetWorld()->
            SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        
        RestartPlayer(Controller);
    }
}

void ACitadelGameModeBase::ResetPlayers()
{
    for (auto It = GetWorld()->GetControllerIterator(); It; It++)
    {
        ResetOnePlayer(It->Get());
    }
}

void ACitadelGameModeBase::ResetOnePlayer(AController* PlayerController)
{
    if (PlayerController && PlayerController->GetPawn())
    PlayerController->GetPawn()->Reset();

    RestartPlayer(PlayerController); // GamemodeBase default function
    SetPlayerColor(PlayerController);
}

void ACitadelGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int32 CurrentTeamID = 1;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        AController* Controller = It->Get();
        if (!Controller) continue;
        
        APlayerStateBase* PlayerState = Cast<APlayerStateBase>(
                Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->SetTeamID(CurrentTeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(CurrentTeamID));
        SetPlayerColor(Controller);

        CurrentTeamID = CurrentTeamID == 1 ? 2 : 1;

    }
    
}

FLinearColor ACitadelGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    else
    {
        UE_LOG(LogTemp, Error, 
            TEXT("There is no that index in GameData.TeamColors array!"));

        return GameData.DefaultTeamColor;
    }
}

void ACitadelGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;

    APlayerGround* Pawn = Cast<APlayerGround>(Controller->GetPawn());
    if (!Pawn) return;

    APlayerStateBase* PlayerState = Cast<APlayerStateBase>(Controller->PlayerState);
    if (!PlayerState) return;

    Pawn->SetPlayerColor(PlayerState->GetTeamColor());

}
