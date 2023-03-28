// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Weapons/WeaponBase.h"

#include "PlayerGround.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UTextRenderComponent;
class UWeaponComponent;

/*
Main Pawn class for Players and bots.
*/
UCLASS()
class CITADEL_API APlayerGround : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerGround();
    APlayerGround(const class FObjectInitializer& ObjectInitializer);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Changes the base color inside the Pawn's Material
    void SetPlayerColor(FLinearColor Color);

    //
    // Pawn control functions
    //

    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);
    void LookUp(float AxisValue);
    void LookRight(float AxisValue);

    //
    // Getters for use in AnimBP
    //

    UFUNCTION(BlueprintPure)
    bool GetCrouching() { return bCrouching; };
    UFUNCTION(BlueprintPure)
    bool GetJogginging() { return bJogging; };
    UFUNCTION(BlueprintPure)
    bool GetSprinting() { return bSprinting; };

    UFUNCTION(BlueprintCallable, Category = "TextRender")
    void UpdateHealthRenderText();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName MaterialColorName =
        "BodyColor";  // name of the node that sets the base color of the Material for the Pawn

    virtual void BeginPlay() override;

    // Disables Pawn's collision and sets spectating mode for Player Controller
    virtual void OnDeath();

private:
    APlayerController* PlayerController;
    bool bCrouching = false;
    bool bJogging = true;
    bool bSprinting = false;

    enum PlayerStances
    {
        Crouching,
        Jogging,
        Sprinting
    };

    UPROPERTY(EditAnywhere)
    UTextRenderComponent* HealthTextRender;
    UPROPERTY(EditAnywhere)
    UWeaponComponent* WeaponComponent;

    void SetupHealthComponent();

    void ToggleStance(PlayerStances Stance);
};
