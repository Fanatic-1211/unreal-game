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
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeaponProjectile> ProjectileClass;

    virtual void Shoot() override;

private:
    FHitResult HitResult;
};
