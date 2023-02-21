// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GroundPlayerHUD.h"

#include "Blueprint/UserWidget.h"



void AGroundPlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    UUserWidget* WidgetHUD = CreateWidget<UUserWidget>(GetWorld(), WidgetHUDClass);

    if (WidgetHUD)
    WidgetHUD->AddToViewport();
}
