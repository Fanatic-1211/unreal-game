// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "WeaponRocketLauncher.generated.h"

class AWeaponProjectile;

UCLASS()
class CITADEL_API AWeaponRocketLauncher : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	virtual void Shoot() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AWeaponProjectile> ProjectileClass;

private:
	FHitResult HitResult;



};
