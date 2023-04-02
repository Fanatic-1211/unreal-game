// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/WeaponBase.h"

#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

#define OUT

DEFINE_LOG_CATEGORY_STATIC(Log_Weapon, All, All);

// Sets default values
AWeaponBase::AWeaponBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneComponent;

    SkeletalMesh = CreateAbstractDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    SkeletalMesh->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
}

AController* AWeaponBase::GetOwnerController()
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return nullptr;

    return OwnerPawn->Controller;
}

void AWeaponBase::GetShotStartEndPoints(
    FHitResult& HitResult, FVector& StartPoint, FVector& EndPoint)
{
    FVector ViewLocation;
    FRotator ViewRotation;

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn->IsPlayerControlled())
    {
        GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
        StartPoint = ViewLocation;
    }
    else
    {
        ViewRotation = SkeletalMesh->GetSocketRotation(MuzzleSocketName);
        StartPoint = SkeletalMesh->GetSocketLocation(MuzzleSocketName);
    }

    FVector TraceDirection = ViewRotation.Vector();
    StartPoint = StartPoint + TraceDirection * 100;  // offset for avoid player collision
    EndPoint = StartPoint + TraceDirection * WeaponRange;

    FCollisionQueryParams TraceParams(TEXT(""), false, GetOwner());  // ignore owner collision
    GetWorld()->LineTraceSingleByChannel(
        OUT HitResult, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility, TraceParams);
}

void AWeaponBase::Shoot()
{
    SpawnEffects();
}

void AWeaponBase::StartFire() {}

void AWeaponBase::StopFire()
{
    GetWorldTimerManager().ClearTimer(DelayBetweenShotsTimerHandle);

    if (!GetWorldTimerManager().TimerExists(DelayBetweenMouseClicksTimerHandle))
    {
        GetWorldTimerManager().SetTimer(DelayBetweenMouseClicksTimerHandle, this,
            &AWeaponBase::Unfire, DelayBetweenShots, false);
    }
}

void AWeaponBase::Unfire()
{
    bNowFiring = false;
}

void AWeaponBase::SpawnEffects()
{
    if (MuzzleFlashParticle && ShotSound)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticle, SkeletalMesh, MuzzleSocketName);
        UGameplayStatics::SpawnSoundAttached(ShotSound, SkeletalMesh, MuzzleSocketName);
    }
    else
    {
        UE_LOG(Log_Weapon, Error, TEXT("%s: Shot Sound or MuzzleFlash Effect doesn't set!"),
            *this->GetName());
    }
}

void AWeaponBase::PrintDebugInfo(FHitResult& HitResult) const
{
    if (HitResult.bBlockingHit)
    {
        UE_LOG(Log_Weapon, VeryVerbose, TEXT("Pew! %s hits %s!"), *GetOwner()->GetName(),
            *HitResult.Actor->GetName());
    }
    else
    {
        UE_LOG(Log_Weapon, VeryVerbose, TEXT("Pew! %s hits nothing!"), *GetOwner()->GetName());
        return;
    }
}
