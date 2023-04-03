// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/PickupHealth.h"
#include "Components/HealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(Log_PickupHealth, All, All);

bool APickupHealth::GivePickupTo(APawn* Pawn)
{
    UE_LOG(Log_PickupHealth, Verbose, TEXT("Health Pickup was taken by %s"), *Pawn->GetName());

    if (TryToRestoreHealth(Pawn))
        return true;
    else
        return false;
}

bool APickupHealth::TryToRestoreHealth(APawn* Pawn)
{
    UHealthComponent* HealthComponent = Pawn->FindComponentByClass<UHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead() ||
        HealthComponent->GetHealth() == HealthComponent->GetMaxHealth())
    {
        return false;
    }

    HealthComponent->AddHealth(HealAmount);

    return true;
}
