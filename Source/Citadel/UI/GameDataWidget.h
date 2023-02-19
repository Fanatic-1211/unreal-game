// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameDataWidget.generated.h"

class ACitadelGameModeBase;
class APlayerStateBase;

UCLASS()
class CITADEL_API UGameDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetPlayerKillsNum();
	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetPlayerDeathsNum();	
	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetTotalRoundsNum();
	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetCurrentRoundNum();
	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetCurrentRoundCountdown();

private:
	ACitadelGameModeBase* GetGameMode();
	APlayerStateBase* GetPlayerState();
};
