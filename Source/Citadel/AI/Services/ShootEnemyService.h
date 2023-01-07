// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ShootEnemyService.generated.h"

class UBlackboardComponent;

UCLASS()
class CITADEL_API UShootEnemyService : public UBTService
{
	GENERATED_BODY()

public:
	UShootEnemyService();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI")
	UBlackboardComponent* Blackboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, 
			uint8* NodeMemory, float DeltaSeconds) override;

private:
};
