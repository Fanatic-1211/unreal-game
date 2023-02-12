// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupHealth.h"
#include "Components/HealthComponent.h"


bool APickupHealth::GivePickupTo(APawn* Pawn)
{
    UE_LOG(LogTemp, Warning, TEXT("Health Pickup was taken by %s"), 
        *Pawn->GetName());

    if(TryToRestoreHealth(Pawn))
    return true;
    else
    return false;
}

bool APickupHealth::TryToRestoreHealth(APawn* Pawn)
{
    UHealthComponent* HealthComponent = Pawn->FindComponentByClass<UHealthComponent>();
    if(!HealthComponent 
        || HealthComponent->IsDead()
        || HealthComponent->GetHealth() == HealthComponent->GetMaxHealth()) 
    return false;

    HealthComponent->AddHealth(HealAmount);

    return true;
}
