// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponRocketLauncher.h"

#include "Kismet/GameplayStatics.h"

#include "Weapons/WeaponProjectile.h"

void AWeaponRocketLauncher::Shoot()
{
    UE_LOG(LogTemp, Warning, TEXT("Shoot!"));
    
	FVector MuzzleSocketLocation = Super::SkeletalMesh->GetSocketLocation(MuzzleSocketName);
    FTransform SpawnLocation(FRotator::ZeroRotator, MuzzleSocketLocation);

    AActor* Projectile = UGameplayStatics::
        BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnLocation);

    UGameplayStatics::FinishSpawningActor(Projectile, SpawnLocation);

}
