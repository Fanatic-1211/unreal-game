// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath); // Делегат, доступный только для С++
DECLARE_MULTICAST_DELEGATE(FOnDamage); 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITADEL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	float GetHealth() { return CurrentHealth; };
	void AddHealth(float Value);

	FOnDeath OnDeath;
	FOnDamage OnDamage;

	UFUNCTION(BlueprintPure)
	bool IsDead();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float CurrentHealth = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UFUNCTION()
	void TakeAnyDamage(AActor* DamageActor, float Damage, 
			const class UDamageType* DamageType, class AController* InstigatedBy,
			AActor* DamageCauser);

};
