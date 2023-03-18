// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UButton;
class UMenuMultiplayer;

UCLASS()
class CITADEL_API UMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;
    UPROPERTY(meta = (BindWidget))
    UButton* PlayOnlineButton;
    UPROPERTY(meta = (BindWidget))
    UButton* ExitGameButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnStartGameClicked();
    UFUNCTION()
    void OnPlayOnlineClicked();
    UFUNCTION()
    void OnQuitGameClicked();
};
