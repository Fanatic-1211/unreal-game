// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/PickupBase.h"
#include "Components/SphereComponent.h"

APickupBase::APickupBase()
{
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

    // Randomize initial rotation angle:
    AddActorLocalRotation(FRotator(0.f, FMath::RandRange(0.f, 350.f), 0.f));
}

void APickupBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.f, RotationSpeed, 0.f));
}

void APickupBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    APawn* Pawn = Cast<APawn>(OtherActor);
    GivePickupTo(Pawn);
    StartRespawnCooldown();
}

void APickupBase::StartRespawnCooldown()
{
    Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Collision->SetVisibility(false, true);

    FTimerHandle RespawnTimer;
    GetWorldTimerManager().SetTimer(RespawnTimer, this, &APickupBase::Respawn, SecondsToRespawn);
}

void APickupBase::Respawn()
{
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Collision->SetVisibility(true, true);
}

bool APickupBase::GivePickupTo(APawn* Pawn)
{
    return false;
}
