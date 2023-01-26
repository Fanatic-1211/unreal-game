// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"

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
    if (!PlayerGround) return;
    AController* PlayerController = PlayerGround->GetController();
    if (!PlayerController) return;

    UGameplayStatics::ApplyDamage(PlayerGround, WeaponDamage, PlayerController, 
            this, nullptr);

	PrintDebugInfo(HitResult);
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
	FVector TraceDirection = ViewRotation.Vector();
	EndPoint = StartPoint + TraceDirection * WeaponRange;
}