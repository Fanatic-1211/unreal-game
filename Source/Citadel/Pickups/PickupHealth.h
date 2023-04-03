// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/PickupBase.h"
#include "PickupHealth.generated.h"

/*
 A pickup that restores the player's health.
 */
UCLASS()
class CITADEL_API APickupHealth : public APickupBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickups")
    float HealAmount = 50.f;

    // Applies the pickup effect to the Pawn.
    virtual bool GivePickupTo(APawn* Pawn) override;

    // Checks the player's health parameters and decides whether to apply the pickup effect.
    // Returns true if the pickup effect was applied.
    virtual bool TryToRestoreHealth(APawn* Pawn);
};
