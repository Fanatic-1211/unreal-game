// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath); // Delegate for C++ only 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamage); // Delegate for Blueprints and C++ 

class UCameraShakeBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITADEL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintPure)
	float GetHealth() { return CurrentHealth; };
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() { return MaxHealth; };
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() { return GetHealth() / MaxHealth; };

	void AddHealth(float Value);

	FOnDeath OnDeath;
	FOnDamage OnDamage;

	UFUNCTION(BlueprintPure)
	bool IsDead();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", 
		meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHealth = 100.f;

	void PlayCameraShake();

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float CurrentHealth = 0.f;

	UPROPERTY(EditAnywhere, Category="VFX")
	TSubclassOf<UCameraShakeBase> CameraShaker;


	UFUNCTION()
	void TakeAnyDamage(AActor* DamageActor, float Damage, 
			const class UDamageType* DamageType, class AController* InstigatedBy,
			AActor* DamageCauser);

	void YellAboutKill(AController* KillerController);
};
