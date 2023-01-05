// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/GetRandomLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#define OUT

UGetRandomLocation::UGetRandomLocation()
{
    NodeName = "Get Random Location";
}

EBTNodeResult::Type UGetRandomLocation::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn); // get Navigation System for current Pawn
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation NavLocation;
    NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), 
            SearchRadius, OUT NavLocation);
    
    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, 
            NavLocation.Location);

    return  EBTNodeResult::Succeeded;
}
