// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponRifle.h"

#define OUT

void AWeaponRifle::CastRay()
{
    FCollisionQueryParams TraceParams(TEXT(""), false, GetOwner());

    GetCharacterPositionParams();

    GetWorld()->LineTraceSingleByObjectType(OUT Hit, CharacterLocation, 
        CharacterRotation.Vector()*WeaponRange + CharacterLocation, 
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams);
}

void AWeaponRifle::GetCharacterPositionParams()
{
    GetWorld()->GetFirstPlayerController()->
        GetPlayerViewPoint(OUT CharacterLocation, OUT CharacterRotation);
}
