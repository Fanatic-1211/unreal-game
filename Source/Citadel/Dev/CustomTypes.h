// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomTypes.generated.h"

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game",
		meta=(ClampMin="1", ClampMax="100"));
	int32 PlayersNum = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game",
		meta=(ClampMin="1", ClampMax="10"));
	int32 RoundsNum = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game",
		meta=(ClampMin="3", ClampMax="300"));
	int32 RoundDuration = 15; // in seconds
};
