// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BrainComponent.h"

#include "AI/AIGround.h"


void AAIGround::BeginPlay()
{
    Super::BeginPlay();

}

void AAIGround::OnDeath()
{
    Super::OnDeath();
    AAIController* AIController = Cast<AAIController>(Controller);
    AIController->GetBrainComponent()->Cleanup();
}
