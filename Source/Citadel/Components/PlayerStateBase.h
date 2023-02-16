// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CITADEL_API APlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	int32 GetDeathsNum() {return this->DeathsNum;}
	void AddDeath() {this->DeathsNum++;}

	int32 GetKillsNum() {return this->KillsNum;}
	void AddKill() {this->KillsNum++;}

	int32 GetTeamID() {return this->TeamID;}
	void SetTeamID(int32 Value) {this->TeamID = Value;}

	FLinearColor GetTeamColor() {return this->TeamColor;}
	void SetTeamColor(FLinearColor Color) {this->TeamColor = Color;}
	
private:
	int32 DeathsNum;
	int32 KillsNum;
	int32 TeamID;
	FLinearColor TeamColor;

};
