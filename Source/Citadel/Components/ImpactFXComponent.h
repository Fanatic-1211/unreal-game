// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ImpactFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITADEL_API UImpactFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UImpactFXComponent();

	void PlayImpactFX(const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, Category="VFX")
	USoundBase* ImpactSound;

		
};
