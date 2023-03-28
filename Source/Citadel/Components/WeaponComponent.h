// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CITADEL_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UWeaponComponent();

    void Shoot();
    void SwitchWeapon();
    void ToggleZoom(bool ZoomON);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<AWeaponBase>> WeaponClasses;

    virtual void BeginPlay() override;

    // Removes Weapon mesh from scene after Player's death
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    int32 ActiveWeaponIdx = 0;  // index of the current weapon in hands

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName ActiveWeaponSocketName = TEXT("WeaponSocket_r");
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName ArmoryWeaponSocketName = TEXT("ArmorySocket");

    TArray<AWeaponBase*> CharacterWeapons;  // weapon in inventory
    AWeaponBase* ActiveWeapon;              // weapon in hands

    void SetupWeapon();  // TODO: need to decompose!
};
