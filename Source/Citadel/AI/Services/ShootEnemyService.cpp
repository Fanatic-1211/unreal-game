// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Services/ShootEnemyService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Components/WeaponComponent.h"

UShootEnemyService::UShootEnemyService()
{
    NodeName = "Shoot Enemy";
}

void UShootEnemyService::TickNode(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* Controller = OwnerComp.GetAIOwner();
    APawn* Pawn = Controller->GetPawn();
    Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Pawn || !Blackboard) return;

    UWeaponComponent* WeaponComponent = Pawn->FindComponentByClass<UWeaponComponent>();
    AActor* EnemyActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
    if (!WeaponComponent || !EnemyActor) return;

    WeaponComponent->StartFire();
    WeaponComponent->StopFire();
}