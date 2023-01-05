// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class CITADEL_API UGetRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UGetRandomLocation();

	/** Function for custom task proper work.  
	 * starts this task, should return Succeeded, Failed or InProgress
	 * (use FinishLatentTask() when returning InProgress) */
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float SearchRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FBlackboardKeySelector AimLocationKey; // don't forget to set proper variable in your BehaviorTree Node (into UE Editor)

private:

};
