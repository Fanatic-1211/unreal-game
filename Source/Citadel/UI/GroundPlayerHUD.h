// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Dev/CustomTypes.h"

#include "GroundPlayerHUD.generated.h"

class UUserWidget;

UCLASS()
class CITADEL_API AGroundPlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetHUDClass;

	virtual void BeginPlay() override;

private:
    void OnMatchStateChanged(CitadelMatchState MatchState);
};
