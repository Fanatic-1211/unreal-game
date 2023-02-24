// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponRifle.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Players/PlayerGround.h"
#include "Components/ImpactFXComponent.h"


#define OUT

AWeaponRifle::AWeaponRifle()
{
	ImpactFXComponent = CreateAbstractDefaultSubobject<UImpactFXComponent>(
		TEXT("ImpactFXComponent"));
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
    
		if (PlayerController)
    	UGameplayStatics::ApplyDamage(PlayerGroundPawn, WeaponDamage, PlayerController, 
            	this, nullptr);
	}

	SpawnTraceFX(TraceEnd);
	ImpactFXComponent->PlayImpactFX(RifleHitResult);

	PrintDebugInfo(RifleHitResult);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, 
	//FColor(0, 255, 0), false, 5.f, 0, 2.f);
}

void AWeaponRifle::GetShotStartEndPoints(FHitResult& HitResult, 
	FVector& StartPoint, FVector& EndPoint) 
{
	Super::GetShotStartEndPoints(HitResult, StartPoint, EndPoint);

	// Add spread at the end of LineTrace:
	const auto HalfConeRadius = FMath::DegreesToRadians(BulletSpread);
    FVector TraceDirection = (EndPoint - StartPoint) / WeaponRange;
    TraceDirection = FMath::VRandCone(TraceDirection, HalfConeRadius);
	EndPoint = StartPoint + TraceDirection * WeaponRange;
}

void AWeaponRifle::SpawnTraceFX(const FVector& EndPoint)
{

	FVector StartPoint = SkeletalMesh->GetSocketLocation(MuzzleSocketName);

	const auto TraceFXComponent = 
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, StartPoint);

	if (TraceFXComponent)
	TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, EndPoint);

}