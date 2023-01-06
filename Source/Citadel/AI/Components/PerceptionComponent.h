// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "PerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class CITADEL_API UPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public: 
	AActor* GetClosestEnemy() const;
};
