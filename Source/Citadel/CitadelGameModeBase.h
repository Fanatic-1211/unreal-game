// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Dev/CustomTypes.h"
#include "CitadelGameModeBase.generated.h"

class AAIController;
UCLASS()
class CITADEL_API ACitadelGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(
		AController* InController) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	FGameData GameData;

private:
	void SpawnBots();

	
};
