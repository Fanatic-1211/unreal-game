// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GroundPlayerHUD.h"

#include "Blueprint/UserWidget.h"

#include "CitadelGameModeBase.h"

void AGroundPlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    UUserWidget* WidgetHUD =
        CreateWidget<UUserWidget>(GetWorld(), WidgetHUDClass);

    if (WidgetHUD) WidgetHUD->AddToViewport();

    ACitadelGameModeBase* GameMode =
        Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());

    GameMode->OnMatchStateChanged.AddUObject(
        this, &AGroundPlayerHUD::OnMatchStateChanged);
}

void AGroundPlayerHUD::OnMatchStateChanged(CitadelMatchState MatchState)
{
    UE_LOG(LogTemp, Display,
        TEXT("GroundPlayerHUD has detected changinging Match State."));
}