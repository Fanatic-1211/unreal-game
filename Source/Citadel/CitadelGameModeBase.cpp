// Fill out your copyright notice in the Description page of Project Settings.


#include "CitadelGameModeBase.h"

#include "AIController.h"



void ACitadelGameModeBase::StartPlay()
{
    Super::StartPlay();

    StartNewRound();
    SpawnBots();
    
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

    RestartPlayer(PlayerController); // GamemodeBase function
}
