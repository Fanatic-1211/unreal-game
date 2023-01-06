// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllerGround.h"
#include "AI/Components/PerceptionComponent.h"
#include "AI/AIGround.h"


AAIControllerGround::AAIControllerGround()
{
    PerceptionComponentGround = CreateDefaultSubobject<UPerceptionComponent>(
        TEXT("PerceptionComponentGround"));
    SetPerceptionComponent(*PerceptionComponentGround);
}

void AAIControllerGround::Tick(float DeltaTime)
{
    ClosestEnemy = PerceptionComponentGround->GetClosestEnemy();
}

void AAIControllerGround::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto AIGround = Cast<AAIGround>(InPawn);
    if (AIGround)
    {
        RunBehaviorTree(AIGround->BehaviorTreeAsset);
    }
}
