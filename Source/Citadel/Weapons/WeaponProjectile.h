// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

class USphereComponent;

UCLASS()
class CITADEL_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponProjectile();

protected:
	UPROPERTY(EditAnywhere, Category="Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category="Weapon")
	UStaticMeshComponent* MeshComponent;

	virtual void BeginPlay() override;

};
