// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerGround.generated.h"

/**
 * 
 */
class UPerceptionComponent;
class UBlackboardComponent;
class URespawnComponent;
UCLASS()
class CITADEL_API AAIControllerGround : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerGround();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UPerceptionComponent* PerceptionComponentGround;

	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	FName EnemyVarNameFromBlackboard = TEXT("CurrentEnemy");
	AActor* GetEnemyFromBlackboard();
	UBlackboardComponent* Blackboard;

	UPROPERTY(EditAnywhere)
	URespawnComponent* RespawnComponent;
};
