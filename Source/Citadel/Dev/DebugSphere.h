// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugSphere.generated.h"

class USceneComponent;

UCLASS()
class CITADEL_API ADebugSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebugSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	float SphereRadius = 300;
	UPROPERTY(EditAnywhere)
	float SphereSegments = 32;
	UPROPERTY(EditAnywhere)
	FColor SphereColor = FColor::Red;

};
