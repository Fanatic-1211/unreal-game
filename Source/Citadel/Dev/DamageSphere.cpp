// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/DamageSphere.h"

#include "Kismet/GameplayStatics.h"

void ADamageSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(),
            Super::SphereRadius, nullptr, {}, this, nullptr, true);
}

