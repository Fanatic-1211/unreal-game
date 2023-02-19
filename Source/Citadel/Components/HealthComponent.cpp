// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "CitadelGamemodeBase.h"

#include "GameFramework/Pawn.h"

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

	CurrentHealth = FMath::Clamp(CurrentHealth + Value, 0.f, MaxHealth);
}


void UHealthComponent::TakeAnyDamage(AActor* DamageActor, float Damage, 
			const class UDamageType* DamageType, class AController* InstigatedBy,
			AActor* DamageCauser)
{
	if (IsDead()) return;

	PlayCameraShake();

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	OnDamage.Broadcast();

	if (IsDead())
	{
		OnDeath.Broadcast(); // оповещаем всех подписаных

		if (InstigatedBy)
		YellAboutKill(InstigatedBy);
		else
		UE_LOG(LogTemp, Error, TEXT("\n%s:UHealthCompnent:TakeAnyDamage\n"
			"Damage Instigator (%s) not specified - kill doesn't count!\n"), 
			*DamageActor->GetHumanReadableName(), *DamageCauser->GetHumanReadableName());
	}
}

bool UHealthComponent::IsDead()
{
	return CurrentHealth <= 0.0f;
}

void UHealthComponent::PlayCameraShake()
{
	APawn* PlayerPawn = Cast<APawn>(GetOwner());
	if (!PlayerPawn) return;

	APlayerController* PlayerController = 
		Cast<APlayerController>(PlayerPawn->GetController());
	if (!PlayerController) return;

	PlayerController->PlayerCameraManager->StartCameraShake(CameraShaker);

}

void UHealthComponent::YellAboutKill(AController* KillerController)
{
	if (!GetWorld()) return;

	ACitadelGameModeBase* GameMode = 
		Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	AController* OwnerController = OwnerPawn->Controller;
	if (OwnerController)
		GameMode->ConfirmKill(KillerController, OwnerController);
	else
		GameMode->ConfirmKill(KillerController, KillerController); // means suicide

}
