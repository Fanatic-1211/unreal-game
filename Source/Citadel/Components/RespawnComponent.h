// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITADEL_API URespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URespawnComponent();

	void Respawn(int32 RespawnTime);

private:
	int32 RespawnCountdown;
	FTimerHandle RespawnTimerHandle;

	void UpdateRespawnTimer();
};
