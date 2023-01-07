// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/BlackboardComponent.h"

#include "AI/AIControllerGround.h"
#include "AI/Components/PerceptionComponent.h"

#include "AI/Services/FindEnemyService.h"

UFindEnemyService::UFindEnemyService()
{
    NodeName = TEXT("Find Enemy");
}


void UFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, 
            float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    const AAIController* Controller = OwnerComp.GetAIOwner();
    const UPerceptionComponent* PerceptionComponent = Cast<UPerceptionComponent>(
        Controller->FindComponentByClass<UPerceptionComponent>());

    if (!PerceptionComponent) return;

    ClosestEnemy = PerceptionComponent->GetClosestEnemy();
    Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, ClosestEnemy);
    
}
