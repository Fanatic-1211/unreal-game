// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class CITADEL_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    virtual float GetMaxSpeed() const override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (ClampMin = "0"));
    float SprintingModifier = 1.5f;
    UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (ClampMin = "0"));
    float CrouchingModifier = 0.4f;
};
