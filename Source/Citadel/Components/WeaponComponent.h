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

	void Shoot();
	void SwitchWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TArray<TSubclassOf<AWeaponBase>> WeaponClasses;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int32 ActiveWeaponIdx = 0;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName ActiveWeaponSocketName = TEXT("WeaponSocket_r");
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName ArmoryWeaponSocketName = TEXT("ArmorySocket");

	TArray<AWeaponBase*> CharacterWeapons;
	AWeaponBase* ActiveWeapon;

	void SetupWeapon();

};
