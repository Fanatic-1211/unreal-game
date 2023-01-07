// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Weapons/WeaponBase.h"

#include "PlayerGround.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UTextRenderComponent;
class UWeaponComponent;


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
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);

	UFUNCTION(BlueprintPure)
	bool GetCrouching() { return IsCrouching; };
	UFUNCTION(BlueprintPure)
	bool GetRunning() { return IsRunning; };

	UFUNCTION(BlueprintCallable, Category="TextRender")
	void UpdateHealthRenderText();

protected:

	virtual void BeginPlay() override;
	virtual void OnDeath();

private:
	AActor* PlayerPawn;
	APlayerController* PlayerController;
	bool IsCrouching;
	bool IsRunning;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* HealthTextRender;

	UPROPERTY(EditAnywhere)
	UWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathAnimMontage;

	void SetupHealthComponent();
	void ToggleCrouch();
	void ToggleRun();


};
