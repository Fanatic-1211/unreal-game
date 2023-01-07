// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FindEnemyService.generated.h"

/**
 * 
 */
UCLASS()
class CITADEL_API UFindEnemyService : public UBTService
{
	GENERATED_BODY()

public:
	UFindEnemyService();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, 
			float DeltaSeconds);

private:
	AActor* ClosestEnemy;
};
