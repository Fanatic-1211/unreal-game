// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Dev/DebugSphere.h"

// Sets default values
ADebugSphere::ADebugSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

}

// Called when the game starts or when spawned
void ADebugSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADebugSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 
			SphereSegments, SphereColor);

}

