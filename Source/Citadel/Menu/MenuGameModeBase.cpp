// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/MenuHUD.h"
#include "Menu/MenuPlayerController.h"

#include "Menu/MenuGameModeBase.h"

AMenuGameModeBase::AMenuGameModeBase()
{
    PlayerControllerClass = AMenuPlayerController::StaticClass();
    HUDClass = AMenuHUD::StaticClass();
}
