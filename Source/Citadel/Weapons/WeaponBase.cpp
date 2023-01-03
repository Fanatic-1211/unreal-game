// Fill out your copyright notice in the Description page of Project Settings.


#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Weapons/WeaponBase.h"

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

void AWeaponBase::Shoot()
{
	PrepareForShot();
	UE_LOG(LogTemp, Warning, TEXT("Pew!"));

	if (!HitResult.bBlockingHit) return;

	UE_LOG(LogTemp, Warning, TEXT("%s was hitted!"), *HitResult.Actor->GetName());
	
}

// Get initial parametars for shot
void AWeaponBase::PrepareForShot()
{
	if (!GetWorld()) return;

	FVector PlayerViewportLocation;
	FRotator PlayerViewportRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewportLocation, PlayerViewportRotation);

	FVector TraceStart = PlayerViewportLocation;
	FVector TraceDirection = PlayerViewportRotation.Vector();
	FVector TraceEnd = TraceStart + TraceDirection * WeaponRange;

	FCollisionQueryParams TraceParams(TEXT(""), false, GetOwner()); // Ignore TraceOwner Collision
	GetWorld()->LineTraceSingleByChannel(OUT HitResult, TraceStart, TraceEnd,
			ECollisionChannel::ECC_Visibility, TraceParams);

	SpawnEffects();

}

void AWeaponBase::SpawnEffects()
{
	if (MuzzleFlashParticle && ShotSound)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticle, SkeletalMesh, MuzzleSocketName);
		UGameplayStatics::SpawnSoundAttached(ShotSound, SkeletalMesh, MuzzleSocketName);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT(
			"%s: Shot Sound or MuzzleFlash Effect doesn't set!"), *this->GetName());
	}
}

