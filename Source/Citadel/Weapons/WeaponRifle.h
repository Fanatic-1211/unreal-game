// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "WeaponRifle.generated.h"

class UImpactFXComponent;
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

	FHitResult RifleHitResult;

	virtual void GetShotStartEndPoints(FHitResult& HitResult, 
		FVector& StartPoint, FVector& EndPoint) override;


private:
	UPROPERTY(EditAnywhere)
	UImpactFXComponent* ImpactFXComponent;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Parameters")
	float WeaponDamage = 10.f;

};
