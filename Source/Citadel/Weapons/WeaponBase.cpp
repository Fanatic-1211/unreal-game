// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"

#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"


#define OUT

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			ViewLocation, ViewRotation);
	}
	else
	{
		ViewRotation = SkeletalMesh->GetSocketRotation(MuzzleSocketName);
	}
	
	StartPoint = SkeletalMesh->GetSocketLocation(MuzzleSocketName);
	FVector TraceDirection = ViewRotation.Vector();
	EndPoint = StartPoint + TraceDirection * WeaponRange;

	FCollisionQueryParams TraceParams(TEXT(""), false, GetOwner()); // Ignore Owner Collision
	GetWorld()->LineTraceSingleByChannel(OUT HitResult, StartPoint, EndPoint,
			ECollisionChannel::ECC_Visibility, TraceParams);
}


void AWeaponBase::Shoot()
{
	if (!GetWorld()) return;
	SpawnEffects();
}

void AWeaponBase::SpawnEffects()
{
	if (MuzzleFlashParticle && ShotSound)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticle, 
				SkeletalMesh, MuzzleSocketName);
		UGameplayStatics::SpawnSoundAttached(ShotSound, SkeletalMesh, 
				MuzzleSocketName);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT(
			"%s: Shot Sound or MuzzleFlash Effect doesn't set!"), *this->GetName());
	}
}

void AWeaponBase::PrintDebugInfo(FHitResult& HitResult) const
{
	if (HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Display, TEXT("Pew! %s hits %s!"), 
				*GetOwner()->GetName(), *HitResult.Actor->GetName());
	}
	else
	{ 
		UE_LOG(LogTemp, Display, TEXT("Pew! %s hits nothing!"), 
				*GetOwner()->GetName());
		return;
	}
}
