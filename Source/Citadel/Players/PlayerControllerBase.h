// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Dev/CustomTypes.h"

#include "PlayerControllerBase.generated.h"

UCLASS()
class CITADEL_API APlayerControllerBase : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void OnPauseGame();
    void OnMatchStateChanged(CitadelMatchState State);
};
