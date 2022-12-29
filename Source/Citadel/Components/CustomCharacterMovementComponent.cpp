// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomCharacterMovementComponent.h"
#include "Players/PlayerGround.h"


float UCustomCharacterMovementComponent::GetMaxSpeed() const 
{
    float MaxSpeed = Super::GetMaxSpeed();
    APlayerGround* Player = Cast<APlayerGround>(GetPawnOwner());

    if (!Player) return MaxSpeed;

    if (Player->GetRunning())
    return MaxSpeed * RunningModifier;
    else if (Player->GetCrouching())
    return MaxSpeed * CrouchingModifier;
    else 
    return MaxSpeed;

}
