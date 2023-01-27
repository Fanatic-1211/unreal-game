// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponRocketLauncher.h"

#include "Kismet/GameplayStatics.h"

#include "Weapons/WeaponProjectile.h"

void AWeaponRocketLauncher::Shoot()
{
    FVector StartPoint;
    FVector EndPoint;
    GetShotStartEndPoints(HitResult, StartPoint, EndPoint);

    FTransform SpawnLocation(FRotator::ZeroRotator, StartPoint);

    AActor* Projectile = UGameplayStatics::
        BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnLocation);

    UGameplayStatics::FinishSpawningActor(Projectile, SpawnLocation);
}
