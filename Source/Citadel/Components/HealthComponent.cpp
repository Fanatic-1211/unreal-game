// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// HealthText = CreateAbstractDefaultSubobject<UTextRenderComponent>(
	// 		TEXT("RenderText"));

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

}

void UHealthComponent::AddHealth(float Value)
{
	CurrentHealth += Value;
}

void UHealthComponent::RemoveHealth(float Value)
{
	CurrentHealth -= Value;
}