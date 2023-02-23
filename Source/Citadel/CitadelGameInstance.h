// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CitadelGameInstance.generated.h"

UCLASS()
class CITADEL_API UCitadelGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    FName GetStartLevelName() { return StartLevelName; };

private:
	UPROPERTY(EditDefaultsOnly)
    FName StartLevelName = NAME_None;
};
