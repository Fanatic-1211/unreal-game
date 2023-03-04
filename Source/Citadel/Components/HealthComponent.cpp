// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"

#include "GameFramework/Pawn.h"
#include "Perception/AISense_Damage.h"

#include "CitadelGamemodeBase.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
        ComponentOwner->OnTakeAnyDamage.AddDynamic(
            this, &UHealthComponent::TakeAnyDamage);
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
        YellAboutKill(InstigatedBy);
        OnDeath.Broadcast();
    }

    MakeFeelAIAboutIncomingDamage(Damage, InstigatedBy);
}

// Notifies AI DamageSense about incoming damage with additional info
void UHealthComponent::MakeFeelAIAboutIncomingDamage(float DamageAmount, AController* DamagerController)
{
    if (!DamagerController || !DamagerController->GetPawn() || !GetOwner()) return;

    UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(),
        DamagerController->GetPawn(), DamageAmount,
        DamagerController->GetPawn()->GetActorLocation(),
        GetOwner()->GetActorLocation());
}

// Notifies GameMode about kill for start victim respawn process and add a score
// point to killer.
void UHealthComponent::YellAboutKill(AController* KillerController)
{
    if (!GetWorld()) return;

    ACitadelGameModeBase* GameMode =
        Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    AController* VictimController = Cast<APawn>(GetOwner())->Controller;

    GameMode->ConfirmKill(KillerController, VictimController);
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