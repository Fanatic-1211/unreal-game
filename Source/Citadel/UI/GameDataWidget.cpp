// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameDataWidget.h"

#include "CitadelGameModeBase.h"
#include "Components/PlayerStateBase.h"


int32 UGameDataWidget::GetPlayerKillsNum()
{
    APlayerStateBase* PlayerState = GetPlayerState();
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 UGameDataWidget::GetPlayerDeathsNum()
{
    APlayerStateBase* PlayerState = GetPlayerState();
    return PlayerState ? PlayerState->GetDeathsNum() : 0;
}

int32 UGameDataWidget::GetTotalRoundsNum()
{
    ACitadelGameModeBase* GameMode = GetGameMode();
    return GameMode ? GameMode->GetTotalRoundsNum() : 0;
}

int32 UGameDataWidget::GetCurrentRoundNum()
{
    ACitadelGameModeBase* GameMode = GetGameMode();
    return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 UGameDataWidget::GetCurrentRoundCountdown()
{
    ACitadelGameModeBase* GameMode = GetGameMode();
    return GameMode ? GameMode->GetRoundCountdown() : 0;
}

ACitadelGameModeBase* UGameDataWidget::GetGameMode()
{
    if (!GetWorld()) return nullptr;

    return Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());
}

APlayerStateBase* UGameDataWidget::GetPlayerState()
{
    return Cast<APlayerStateBase>(GetOwningPlayer()->PlayerState);
}
