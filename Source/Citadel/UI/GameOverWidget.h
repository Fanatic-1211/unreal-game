// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Dev/CustomTypes.h"

#include "GameOverWidget.generated.h"

class UVerticalBox;

UCLASS()
class CITADEL_API UGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual bool Initialize() override; // BeginPlay analog for Widgets

protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStatBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStatisticWidgetClass;

protected:
    void OnMatchStateChanged(CitadelMatchState State);
    void UpdatePlayerStatistic();
};
