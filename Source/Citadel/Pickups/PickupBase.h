// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;

UCLASS()
class CITADEL_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

	virtual void BeginPlay() override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditDefaultsOnly)
	float SecondsToRespawn = 5.f;

	virtual void StartRespawnCooldown();
	virtual void Respawn();
	virtual bool GivePickupTo(APawn* Pawn);

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 3.f;
};
