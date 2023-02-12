// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/PickupBase.h"
#include "PickupHealth.generated.h"

/**
 * 
 */
UCLASS()
class CITADEL_API APickupHealth : public APickupBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickups")
	float HealAmount = 50.f;

	virtual bool GivePickupTo(APawn* Pawn) override;
	virtual bool TryToRestoreHealth(APawn* Pawn);

};
