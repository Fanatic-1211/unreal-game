// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponProjectile.h"

#include "Components/SphereComponent.h"

AWeaponProjectile::AWeaponProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(CollisionComponent);
}

void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
