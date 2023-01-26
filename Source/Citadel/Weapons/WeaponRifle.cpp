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
	GetShotStartEndPoints(OUT TraceStart, OUT TraceEnd);

	FCollisionQueryParams TraceParams(TEXT(""), false, GetOwner()); // Ignore Owner Collision
	GetWorld()->LineTraceSingleByChannel(OUT HitResult, TraceStart, TraceEnd,
			ECollisionChannel::ECC_Visibility, TraceParams);

    APlayerGround* PlayerGround = Cast<APlayerGround>(HitResult.GetActor());
    
	if (PlayerGround)
	{
    	AController* PlayerController = PlayerGround->GetController();
    
		if (PlayerController)
    	UGameplayStatics::ApplyDamage(PlayerGround, WeaponDamage, PlayerController, 
            	this, nullptr);
	}

	PrintDebugInfo(HitResult);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, 
		FColor(0, 255, 0), false, 5.f, 0, 2.f);
}

void AWeaponRifle::GetShotStartEndPoints(FVector& StartPoint, FVector& EndPoint)
{
	FVector ViewLocation;
	FRotator ViewRotation;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn->IsPlayerControlled())
	{
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = Super::SkeletalMesh->GetSocketLocation(MuzzleSocketName);
	}
	
	StartPoint = ViewLocation;
	const auto HalfConeRadius = FMath::DegreesToRadians(BulletSpread); 
	FVector TraceDirection = FMath::VRandCone(ViewRotation.Vector(), HalfConeRadius);
	EndPoint = StartPoint + TraceDirection * WeaponRange;
}