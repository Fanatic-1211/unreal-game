// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "Players/PlayerGround.h"

#include "Weapons/WeaponRifle.h"

#define OUT


void AWeaponRifle::Shoot() 
{
    Super::Shoot();

	FVector TraceStart;
	FVector TraceEnd;
	GetShotStartEndPoints(OUT RifleHitResult, OUT TraceStart, OUT TraceEnd);


    APlayerGround* PlayerGround = Cast<APlayerGround>(RifleHitResult.GetActor());
    
	if (PlayerGround)
	{
    	AController* PlayerController = PlayerGround->GetController();
    
		if (PlayerController)
    	UGameplayStatics::ApplyDamage(PlayerGround, WeaponDamage, PlayerController, 
            	this, nullptr);
	}

	PrintDebugInfo(RifleHitResult);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, 
		FColor(0, 255, 0), false, 5.f, 0, 2.f);
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