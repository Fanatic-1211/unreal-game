// Fill out your copyright notice in the Description page of Project Settings.

#include "Players/PlayerControllerBase.h"

#include "CitadelGameModeBase.h"

void APlayerControllerBase::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent) return;

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &APlayerControllerBase::OnPauseGame)
        .bExecuteWhenPaused = true;
}

void APlayerControllerBase::OnPauseGame()
{
    if (!GetWorld()) return;

    ACitadelGameModeBase* GameMode = Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());

    if (GameMode)
    {
        if (GameMode->GetMatchState() == CitadelMatchState::Pause)
            GameMode->ClearPause();
        else
            GameMode->SetPause(this);
    }
}

void APlayerControllerBase::OnMatchStateChanged(CitadelMatchState State)
{
    if (State == CitadelMatchState::InProgress)
        bShowMouseCursor = false;
    else
        bShowMouseCursor = true;
}

void APlayerControllerBase::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeGameOnly());

    check(GetWorld());

    ACitadelGameModeBase* GameMode = Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());

    if (GameMode)
    {
        GameMode->OnMatchStateChanged.AddUObject(this, &APlayerControllerBase::OnMatchStateChanged);
    }
}
