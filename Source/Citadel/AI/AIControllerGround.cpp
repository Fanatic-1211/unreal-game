// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllerGround.h"
#include "AI/AIGround.h"


void AAIControllerGround::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto AIGround = Cast<AAIGround>(InPawn);
    if (AIGround)
    {
        RunBehaviorTree(AIGround->BehaviorTreeAsset);
    }
}
