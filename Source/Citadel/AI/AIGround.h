// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Players/PlayerGround.h"
#include "AIGround.generated.h"

/**
 * 
 */
class UBehaviorTree;
UCLASS()
class CITADEL_API AAIGround : public APlayerGround
{
	GENERATED_BODY()
	
public:
	// virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTreeAsset;
};
