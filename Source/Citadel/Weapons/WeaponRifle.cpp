// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/WeaponRifle.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Players/PlayerGround.h"
#include "Components/ImpactFXComponent.h"
#include "Players/PlayerControllerGround.h"

#define OUT

DEFINE_LOG_CATEGORY_STATIC(Log_WeaponRifle, All, All);

AWeaponRifle::AWeaponRifle()
{
    ImpactFXComponent =
        CreateAbstractDefaultSubobject<UImpactFXComponent>(TEXT("ImpactFXComponent"));
}

void AWeaponRifle::Shoot()
{
    Super::Shoot();

    FVector TraceStart;
    FVector TraceEnd;
    GetShotStartEndPoints(OUT RifleHitResult, OUT TraceStart, OUT TraceEnd);

    APlayerGround* PlayerGroundPawn = Cast<APlayerGround>(RifleHitResult.GetActor());

    if (PlayerGroundPawn)
    {
        AController* PlayerController = Cast<APawn>(GetOwner())->Controller;

        UE_LOG(Log_WeaponRifle, VeryVerbose, TEXT("%s has been hit in %s!"),
            *PlayerController->GetName(), *RifleHitResult.BoneName.ToString());

        if (PlayerController)
        {
            if (RifleHitResult.BoneName == "head")  // is headshot?
            {
                UGameplayStatics::ApplyDamage(PlayerGroundPawn, WeaponDamage * HeadshotMultiplier,
                    PlayerController, this, nullptr);
            }
            else
            {
                UGameplayStatics::ApplyDamage(
                    PlayerGroundPawn, WeaponDamage, PlayerController, this, nullptr);
            }
        }
    }

    SpawnTraceFX(TraceEnd);
    ImpactFXComponent->PlayImpactFX(RifleHitResult);

    PrintDebugInfo(RifleHitResult);
}

void AWeaponRifle::StartFire()
{
    if (bNowFiring) return;
    bNowFiring = true;
    Shoot();
    GetWorldTimerManager().SetTimer(
        DelayBetweenShotsTimerHandle, this, &ThisClass::Shoot, DelayBetweenShots, true);
}

void AWeaponRifle::StopFire()
{
    Super::StopFire();
}

void AWeaponRifle::GetShotStartEndPoints(
    FHitResult& HitResult, FVector& StartPoint, FVector& EndPoint)
{
    Super::GetShotStartEndPoints(HitResult, StartPoint, EndPoint);

    // Add spread at the end of LineTrace:
    const auto HalfConeRadius = FMath::DegreesToRadians(BulletSpread);
    FVector TraceDirection = (EndPoint - StartPoint) / WeaponRange;
    TraceDirection = FMath::VRandCone(TraceDirection, HalfConeRadius);
    EndPoint = StartPoint + TraceDirection * WeaponRange;
}

bool AWeaponRifle::ZoomFOV(bool ZoomON)
{
    APlayerControllerGround* PlayerController = Cast<APlayerControllerGround>(GetOwnerController());
    if (!PlayerController) return false;

    const TInterval<float> FOV(50.f, 90.f);

    if (ZoomON)
    {
        PlayerController->PlayerCameraManager->SetFOV(FOV.Min);
    }
    else
    {
        PlayerController->PlayerCameraManager->SetFOV(FOV.Max);
    }

    return true;
}

void AWeaponRifle::SpawnTraceFX(const FVector& EndPoint)
{

    FVector StartPoint = SkeletalMesh->GetSocketLocation(MuzzleSocketName);

    const auto TraceFXComponent =
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, StartPoint);

    if (TraceFXComponent) TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, EndPoint);
}