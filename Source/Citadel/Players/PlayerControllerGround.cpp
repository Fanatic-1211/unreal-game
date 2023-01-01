// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/UserWidget.h"

#include "Players/PlayerControllerGround.h"


void APlayerControllerGround::BeginPlay()
{
    UUserWidget* AimCross = CreateWidget(this, AimCrossClass);
    if (AimCross)
    AimCross->AddToViewport();
}