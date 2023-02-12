// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupBase.h"
#include "Components/SphereComponent.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(50.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(Collision);

}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	// Randomize initial rotation angle
	AddActorLocalRotation(FRotator(0.f, FMath::RandRange(0.f, 350.f), 0.f));
}



void APickupBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("Pickup was taken!"));
	APawn* Pawn = Cast<APawn>(OtherActor);
	GivePickupTo(Pawn);
	StartRespawnCooldown();
}

void APickupBase::StartRespawnCooldown()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->SetVisibility(false, true);

	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, this, 
		&APickupBase::Respawn, SecondsToRespawn);
}

void APickupBase::Respawn()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetVisibility(true, true);
	UE_LOG(LogTemp, Warning, TEXT("Respawned!"));
}

bool APickupBase::GivePickupTo(APawn* Pawn)
{
	return false;
}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, RotationSpeed, 0.f));
}
