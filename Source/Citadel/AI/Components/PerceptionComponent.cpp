// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Components/PerceptionComponent.h"

#include "AIController.h"
#include "Perception/AISense_Sight.h"

#include "Components/HealthComponent.h"


AActor* UPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceivedActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
    if (PerceivedActors.Num() == 0) return nullptr;

    const AAIController* Controller = Cast<AAIController>(GetOwner());
    const APawn* Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestEnemy = nullptr;

    for (const auto PerceiveActor : PerceivedActors)
    {
        UHealthComponent* HealthComponent = 
            PerceiveActor->FindComponentByClass<UHealthComponent>();
        
        if (HealthComponent && !HealthComponent->IsDead()) 
        {

            float CurrentDistance = (
                Pawn->GetActorLocation() - PerceiveActor->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestEnemy = PerceiveActor;
            }
        }
    }

    return BestEnemy;

}