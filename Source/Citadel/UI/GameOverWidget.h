// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Dev/CustomTypes.h"

#include "GameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class CITADEL_API UGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

public:

protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStatBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStatisticWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UButton* ResetButton;

    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    virtual void NativeOnInitialized() override; // BeginPlay analog for Widgets

private:
    void OnMatchStateChanged(CitadelMatchState State);
    void UpdatePlayerStatistic();

    UFUNCTION()
    void OnResetLevel();

    UFUNCTION()
    void OnClickMainMenu();
};
