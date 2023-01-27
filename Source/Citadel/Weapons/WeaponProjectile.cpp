// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AWeaponProjectile::AWeaponProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(CollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		"ProjectileMovementComponent");
}

void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMovementComponent)
	ProjectileMovementComponent->Velocity =
		ShotDirection * ProjectileMovementComponent->InitialSpeed;

	SetLifeSpan(3.f);
	
}
