// Fill out your copyright notice in the Description page of Project Settings.


#include "CitadelGameModeBase.h"

#include "AIController.h"



void ACitadelGameModeBase::StartPlay()
{
    Super::StartPlay();
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
