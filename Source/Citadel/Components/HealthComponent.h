// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITADEL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	float GetHealth() { return CurrentHealth; };
	void AddHealth(float Value);
	void RemoveHealth(float Value); 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float CurrentHealth = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* HealthText;

};
