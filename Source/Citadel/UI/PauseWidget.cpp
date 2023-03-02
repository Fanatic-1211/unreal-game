// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "CitadelGameModeBase.h"


void UPauseWidget::NativeOnInitialized()
{
    Super::Initialize();
    ACitadelGameModeBase* GameMode =
        Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());

    if (GameMode)
    {
        GameMode->OnMatchStateChanged.AddUObject(
            this, &UPauseWidget::OnMatchStateChanged);
    }

    if (ResumeButton)
        ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickResumeButton);

    if (MainMenuButton)
        MainMenuButton->OnClicked.AddDynamic(
            this, &UPauseWidget::OnClickMainMenuButton);
}

void UPauseWidget::OnMatchStateChanged(CitadelMatchState State)
{
    
}

void UPauseWidget::OnClickMainMenuButton()
{
    FName MainMenuName = "MainMenu";
    UGameplayStatics::OpenLevel(this, MainMenuName);
}

void UPauseWidget::OnClickResumeButton()
{
    ACitadelGameModeBase* GameMode =
        Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());

    if (GameMode->GetMatchState() == CitadelMatchState::Pause)
        GameMode->ClearPause();
}
