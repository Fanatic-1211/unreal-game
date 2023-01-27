// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USceneComponent;
class USkeletalMeshComponent;

UCLASS()
class CITADEL_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	virtual void Shoot();

protected:
	UPROPERTY(EditAnywhere)
	FName MuzzleSocketName = TEXT("Muzzle_Socket");
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditDefaultsOnly, Category="Weapon Parameters")
	int32 WeaponRange = 2000;
	UPROPERTY(EditAnywhere, Category = "FX")
	UParticleSystem* MuzzleFlashParticle;
	UPROPERTY(EditAnywhere, Category="FX")
	USoundBase* ShotSound;

	virtual void SpawnEffects();
	virtual void GetShotStartEndPoints(FHitResult& HitResult, 
		FVector& StartPoint, FVector& EndPoint);
	virtual void PrintDebugInfo(FHitResult& HitResult) const;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
