// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "WeaponRifle.generated.h"

class UImpactFXComponent;
class UNiagaraSystem;
UCLASS()
class CITADEL_API AWeaponRifle : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeaponRifle();
	virtual void Shoot() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BulletSpread = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	FString TraceTargetName = "TraceTarget";

	FHitResult RifleHitResult;

	virtual void GetShotStartEndPoints(FHitResult& HitResult, 
		FVector& StartPoint, FVector& EndPoint) override;


private:
	UPROPERTY(EditAnywhere)
	UImpactFXComponent* ImpactFXComponent;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Parameters")
	float WeaponDamage = 10.f;

	void SpawnTraceFX(const FVector& StartPoint, const FVector& EndPoint);

};
