// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Citadel/Players/PlayerBase.h"
#include "Weapons/WeaponBase.h"

#include "PlayerGround.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UTextRenderComponent;


UCLASS()
class CITADEL_API APlayerGround : public ACharacter
{
	GENERATED_BODY()
	
public:
	APlayerGround();
	APlayerGround(const class FObjectInitializer& ObjectInitializer);

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

protected:
	virtual void BeginPlay() override;


private:
	UHealthComponent* HealthComponent;
	AActor* PlayerPawn;
	APlayerController* PlayerController;
	bool IsCrouching;
	bool IsRunning;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* HealthTextRender;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY()
	AWeaponBase* Weapon;

	void SetupWeapon();
	void SetupHealthComponent();
	void UpdateHealthRenderText();
	void OnDeath();
	void ToggleCrouch();
	void ToggleRun();

};
