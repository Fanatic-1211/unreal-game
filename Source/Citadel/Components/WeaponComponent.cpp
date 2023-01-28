// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Weapons/WeaponRifle.h"
#include "Weapons/WeaponRocketLauncher.h"
#include "Players/PlayerGround.h"

#include "Components/WeaponComponent.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupWeapon();
}

void UWeaponComponent::SetupWeapon()
{
	APlayerGround* Player = Cast<APlayerGround>(GetOwner());
	if (!Player) return;

	if (WeaponClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Error,
			TEXT("%s: Default Weapons has not set!"), *Player->GetName());
		return;
	}

	FAttachmentTransformRules AttachmentRules (EAttachmentRule::SnapToTarget, false);

	for (auto WeaponClass : WeaponClasses)
	{
		AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

		Weapon->AttachToComponent(
			Player->GetMesh(), 
			FAttachmentTransformRules::KeepRelativeTransform, 
			ArmoryWeaponSocketName);

		Weapon->SetOwner(Player);
		CharacterWeapons.Add(Weapon);
	}

	ActiveWeapon = CharacterWeapons[ActiveWeaponIdx];
	if (!ActiveWeapon) 
	{
		UE_LOG(LogTemp, Error,
			TEXT("%s: Cant get active weapon!"), *Player->GetName());
		return;
	}

	ActiveWeapon->AttachToComponent(
			Player->GetMesh(), 
			FAttachmentTransformRules::KeepRelativeTransform, 
			ActiveWeaponSocketName);
}

void UWeaponComponent::Shoot()
{
	if (AWeaponRifle* WeaponCasted = Cast<AWeaponRifle>(ActiveWeapon))
	WeaponCasted->Shoot();
	
	if (AWeaponRocketLauncher* WeaponCasted = Cast<AWeaponRocketLauncher>(ActiveWeapon))
	WeaponCasted->Shoot();
}

void UWeaponComponent::SwitchWeapon()
{
	APlayerGround* Player = Cast<APlayerGround>(GetOwner());
	if (!Player) return;

	UE_LOG(LogTemp, Warning, TEXT("Switch"));
	ActiveWeapon->AttachToComponent(
			Player->GetMesh(), 
			FAttachmentTransformRules::KeepRelativeTransform, 
			ArmoryWeaponSocketName);

	if (ActiveWeaponIdx == 1) 
	ActiveWeaponIdx = 0;
	else if (ActiveWeaponIdx == 0) 
	ActiveWeaponIdx = 1;


	ActiveWeapon = CharacterWeapons[ActiveWeaponIdx];

	ActiveWeapon->AttachToComponent(
			Player->GetMesh(), 
			FAttachmentTransformRules::KeepRelativeTransform, 
			ActiveWeaponSocketName);
}
