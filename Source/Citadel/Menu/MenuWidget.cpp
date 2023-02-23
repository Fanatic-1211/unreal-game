// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "Menu/MenuWidget.h"



void UMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(StartGameButton)
        StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGame);
}

void UMenuWidget::OnStartGame()
{
    const FName StartupLevel = FName("TestLevel");
    UGameplayStatics::OpenLevel(this, StartupLevel);
}
