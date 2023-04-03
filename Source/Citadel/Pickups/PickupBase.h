// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;

/*
Parent class for all pickup classes.
*/
UCLASS()
class CITADEL_API APickupBase : public AActor
{
    GENERATED_BODY()

public:
    APickupBase();

    virtual void BeginPlay() override;

protected:
    virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;  // default UE notifier

    // Applies the pickup effect to the Pawn.
    virtual bool GivePickupTo(APawn* Pawn);

    // ----------
    // PICKUP RESPAWN

    UPROPERTY(EditDefaultsOnly)
    float SecondsToRespawn = 5.f;

    // Disables pickup collision and runs timer for call Respawn function.
    virtual void StartRespawnCooldown();
    // Enables pickup collision and visibility
    virtual void Respawn();

private:
    UPROPERTY(EditAnywhere)
    USphereComponent* Collision;

    UPROPERTY(EditAnywhere)
    float RotationSpeed = 3.f;
};
