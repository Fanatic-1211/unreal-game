// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Citadel/Players/PlayerBase.h"

#include "PlayerGround.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UTextRenderComponent;

UCLASS()
class CITADEL_API APlayerGround : public APlayerBase
{
	GENERATED_BODY()
	
public:
	APlayerGround();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);

protected:
	virtual void BeginPlay() override;


private:
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* HealthTextRender;

	void UpdateHealthRenderText();
};
