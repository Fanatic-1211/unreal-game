// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/MenuWidget.h"

#include "Menu/MenuHUD.h"

void AMenuHUD::BeginPlay()
{
    UMenuWidget* MenuWidget =
        CreateWidget<UMenuWidget>(GetWorld(), MenuWidgetClass);

    if (MenuWidget) MenuWidget->AddToViewport();
}
