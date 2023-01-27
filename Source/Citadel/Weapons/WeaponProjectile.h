// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class CITADEL_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponProjectile();

	void SetShotDirection(FVector& Direction) {ShotDirection = Direction;}

protected:
	UPROPERTY(EditAnywhere, Category="Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category="Weapon")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category="Weapon")
	UProjectileMovementComponent* ProjectileMovementComponent;

	virtual void BeginPlay() override;

private:
	FVector ShotDirection;
};
