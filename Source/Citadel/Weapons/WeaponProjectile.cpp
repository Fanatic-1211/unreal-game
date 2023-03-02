// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "Components/ImpactFXComponent.h"

AWeaponProjectile::AWeaponProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly); // notify only about colliding with other objects
	CollisionComponent->SetCollisionResponseToAllChannels(
		ECollisionResponse::ECR_Block); // collision is blocked by all channels

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(CollisionComponent);

	ImpactFXComponent = CreateAbstractDefaultSubobject<UImpactFXComponent>(
		TEXT("ImpactFXComponent"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		"ProjectileMovementComponent");
}

void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(ProjectileMovementComponent);
	check(CollisionComponent);

	AlignForwardVectorWithOwner();

	ProjectileMovementComponent->Velocity =
		ShotDirection * ProjectileMovementComponent->InitialSpeed;

	SetLifeSpan(3.f);

	CollisionComponent->OnComponentHit.AddDynamic(this, 
		&AWeaponProjectile::OnProjectileHit);

}

void AWeaponProjectile::OnProjectileHit(
		UPrimitiveComponent* HitComponent, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, 
		const FHitResult& Hit)
{
	if (!GetWorld()) return;

	ProjectileMovementComponent->StopMovementImmediately();

	ImpactFXComponent->PlayImpactFX(Hit);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	AController* OwnerController = OwnerPawn->Controller;

	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, 
		GetActorLocation(), DamageRadius,
		UDamageType::StaticClass(), {}, this, 
		OwnerController, false);

	// DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 36, 
	// 	FColor::Red, false, 0.5f);

	Destroy();
}

void AWeaponProjectile::AlignForwardVectorWithOwner()
{
	this->SetActorRotation(GetOwner()->GetActorForwardVector().Rotation());
}
