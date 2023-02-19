// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Players/PlayerControllerBase.h"

#include "PlayerControllerGround.generated.h"

class URespawnComponent;

UCLASS()
class CITADEL_API APlayerControllerGround : public APlayerControllerBase
{
	GENERATED_BODY()

public:
	APlayerControllerGround();
	
protected:
	virtual void BeginPlay() override;

private:
	URespawnComponent* RespawnComponent;
};

