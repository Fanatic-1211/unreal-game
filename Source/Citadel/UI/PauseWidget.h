// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Dev/CustomTypes.h"

#include "PauseWidget.generated.h"

class UButton;

UCLASS()
class CITADEL_API UPauseWidget : public UUserWidget
{
    GENERATED_BODY()

public:

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    virtual void NativeOnInitialized() override; // BeginPlay analog for Widgets

private:
    void OnMatchStateChanged(CitadelMatchState State);

    UFUNCTION()
    void OnClickMainMenuButton();

    UFUNCTION()
    void OnClickResumeButton();
};
