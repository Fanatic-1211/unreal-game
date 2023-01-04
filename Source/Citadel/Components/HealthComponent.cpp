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
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(
				this, &UHealthComponent::TakeAnyDamage);
	}

}

void UHealthComponent::AddHealth(float Value)
{
	CurrentHealth += Value;
}


void UHealthComponent::TakeAnyDamage(AActor* DamageActor, float Damage, 
			const class UDamageType* DamageType, class AController* InstigatedBy,
			AActor* DamageCauser)
{
	if (IsDead()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	OnDamage.Broadcast();

	if (IsDead())
	{
		OnDeath.Broadcast(); // оповещаем всех подписаных
	}
}

bool UHealthComponent::IsDead()
{
	return CurrentHealth <= 0.0f;
}

