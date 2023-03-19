// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/MenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "CitadelGameInstance.h"

void UMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
        StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGameClicked);

    if (ExitGameButton) ExitGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitGameClicked);
}

void UMenuWidget::OnStartGameClicked()
{
    if (!GetWorld()) return;

    UCitadelGameInstance* GameInstance = GetWorld()->GetGameInstance<UCitadelGameInstance>();
    if (!GameInstance) return;

    if (GameInstance->GetStartLevelName().IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("Startup level name not set!"));
        return;
    }

    UGameplayStatics::OpenLevel(this, GameInstance->GetStartLevelName());
}

void UMenuWidget::OnQuitGameClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
