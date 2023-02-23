// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "CitadelGameInstance.h"

#include "Menu/MenuWidget.h"

void UMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
        StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGame);
}

void UMenuWidget::OnStartGame()
{
    if (!GetWorld()) return;

    UCitadelGameInstance* GameInstance =
        GetWorld()->GetGameInstance<UCitadelGameInstance>();
    if (!GameInstance) return;

    if (GameInstance->GetStartLevelName().IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("Startup level name not set!"));
        return;
    }

    UGameplayStatics::OpenLevel(this, GameInstance->GetStartLevelName());
}
