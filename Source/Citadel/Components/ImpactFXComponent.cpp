// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ImpactFXComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UImpactFXComponent::UImpactFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UImpactFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticle, Hit.ImpactPoint);
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.ImpactPoint);
}

