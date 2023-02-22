// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatisticWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"


void UStatisticWidget::SetPlayerName(const FText& Text)
{
    if (!PlayerNameTextBlock) return;
    PlayerNameTextBlock->SetText(Text);
}

void UStatisticWidget::SetKills(const FText& Text)
{
    if (!KillsTextBlock) return;
    KillsTextBlock->SetText(Text);
}

void UStatisticWidget::SetDeaths(const FText& Text)
{
    if (!DeathsTextBlock) return;
    DeathsTextBlock->SetText(Text);
}

void UStatisticWidget::SetTeam(const FText& Text)
{
    if (!PlayerNameTextBlock) return;
    TeamTextBlock->SetText(Text);
}

void UStatisticWidget::SetPlayerIndicatorVisibility(bool IsVisible)
{
    if (!PlayerIndicatorImage) return;
    PlayerIndicatorImage->SetVisibility(
        IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
