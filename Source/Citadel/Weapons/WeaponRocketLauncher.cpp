// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponRocketLauncher.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "Weapons/WeaponProjectile.h"

void AWeaponRocketLauncher::Shoot()
{
    Super::Shoot();

    FVector StartPoint;
    FVector EndPoint;
    GetShotStartEndPoints(HitResult, StartPoint, EndPoint);
    EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : EndPoint;

    FVector Direction = (EndPoint - StartPoint).GetSafeNormal();

    FTransform SpawnLocation(FRotator::ZeroRotator, StartPoint);

    AWeaponProjectile* Projectile = GetWorld()->SpawnActorDeferred<AWeaponProjectile>(
        ProjectileClass, SpawnLocation);
    // DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor(0, 255, 0), false,
    //     5.f, 0, 2.f);

    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnLocation);
    }
}
