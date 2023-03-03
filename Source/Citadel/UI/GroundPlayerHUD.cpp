// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GroundPlayerHUD.h"

#include "Blueprint/UserWidget.h"

#include "CitadelGameModeBase.h"

void AGroundPlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    WidgetPause = CreateWidget<UUserWidget>(GetWorld(), WidgetPauseClass);
    WidgetHUD = CreateWidget<UUserWidget>(GetWorld(), WidgetHUDClass);
    WidgetGameOver = CreateWidget<UUserWidget>(GetWorld(), WidgetGameOverClass);

    if (WidgetHUD) WidgetHUD->AddToViewport();
    if (WidgetHUD) WidgetPause->AddToViewport();
    if (WidgetHUD) WidgetGameOver->AddToViewport();

    ACitadelGameModeBase* GameMode =
        Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());

    GameMode->OnMatchStateChanged.AddUObject(
        this, &AGroundPlayerHUD::OnMatchStateChanged);
}

void AGroundPlayerHUD::OnMatchStateChanged(CitadelMatchState MatchState)
{
    UE_LOG(LogTemp, Display,
        TEXT("GroundPlayerHUD has detected changinging MatchState: %s"),
        *UEnum::GetValueAsString(MatchState));

    if (MatchState == CitadelMatchState::Pause)
    {
        if (WidgetPause) WidgetPause->SetVisibility(ESlateVisibility::Visible);
    }
    if (MatchState == CitadelMatchState::InProgress)
    {
        if (WidgetPause) WidgetPause->SetVisibility(ESlateVisibility::Hidden);
        if (WidgetPause) WidgetGameOver->SetVisibility(ESlateVisibility::Hidden);
    }
    if (MatchState == CitadelMatchState::GameOver)
    {
        if (WidgetPause) WidgetPause->SetVisibility(ESlateVisibility::Hidden);
        if (WidgetPause) WidgetGameOver->SetVisibility(ESlateVisibility::Visible);
    }
}