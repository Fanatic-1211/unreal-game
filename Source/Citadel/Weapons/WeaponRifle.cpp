// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"

#include "Players/PlayerGround.h"

#include "Weapons/WeaponRifle.h"

#define OUT


void AWeaponRifle::Shoot() 
{
    Super::Shoot();
    APlayerGround* PlayerGround = Cast<APlayerGround>(Super::HitResult.GetActor());
    if (!PlayerGround) return;
    AController* PlayerController = PlayerGround->GetController();
    if (!PlayerController) return;

    UGameplayStatics::ApplyDamage(PlayerGround, WeaponDamage, PlayerController, 
            this, nullptr);

}