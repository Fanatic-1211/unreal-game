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


	Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
	if (!Weapon)
	{
	UE_LOG(LogTemp, Error, TEXT("%s: Default Weapon has not set!"), *Player->GetName());
	return;
	}
	Weapon->AttachToComponent(Player->GetMesh(), 
		FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket_r"));
	Weapon->SetOwner(Player);
}

void UWeaponComponent::Shoot()
{
	if (AWeaponRifle* WeaponCasted = Cast<AWeaponRifle>(Weapon))
	WeaponCasted->Shoot();
	
	if (AWeaponRocketLauncher* WeaponCasted = Cast<AWeaponRocketLauncher>(Weapon))
	WeaponCasted->Shoot();
}

