// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/UserWidget.h"

#include "Players/PlayerControllerGround.h"
#include "Components/RespawnComponent.h"


APlayerControllerGround::APlayerControllerGround()
{
    RespawnComponent = CreateAbstractDefaultSubobject<URespawnComponent>(
                TEXT("RespawnComponent"));
}

void APlayerControllerGround::BeginPlay()
{
    Super::BeginPlay();
}