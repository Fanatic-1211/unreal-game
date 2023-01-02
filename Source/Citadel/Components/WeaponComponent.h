// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITADEL_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	UPROPERTY()
	AWeaponBase* Weapon;

	void Shoot();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeaponBase> WeaponClass;

	void SetupWeapon();

};
