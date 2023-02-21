// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/RespawnComponent.h"

#include "CitadelGameModeBase.h"

// Sets default values for this component's properties
URespawnComponent::URespawnComponent()
{
    // Set this component to be initialized when the game starts, and to be
    // ticked every frame.  You can turn these features off to improve
    // performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
}

void URespawnComponent::UpdateRespawnTimer()
{
    RespawnCountdown--;

    if (RespawnCountdown == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
        ACitadelGameModeBase* GameMode =
            Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());
        if (!GameMode) return;

        GameMode->RequestRespawn(Cast<AController>(GetOwner()));
    }
}

void URespawnComponent::Respawn(int32 RespawnTime)
{
    if (!GetWorld()) return;
    RespawnCountdown = RespawnTime;

    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this,
        &URespawnComponent::UpdateRespawnTimer, 1.f, true);
}