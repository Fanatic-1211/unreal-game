// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Components/PerceptionComponent.h"
#include "AIController.h"
#include "Components/HealthComponent.h"
#include "Perception/AISense_Sight.h"


AActor* UPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceivedActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
    if (PerceivedActors.Num() == 0) return nullptr;

    const AAIController* Controller = Cast<AAIController>(GetOwner());
    const APawn* Pawn = Controller->GetPawn();

    float BestDistance = MAX_FLT;
    AActor* BestEnemy = nullptr;

    for (const auto PerceiveActor : PerceivedActors)
    {
        UHealthComponent* HealthComponent = Cast<UHealthComponent>(
                Pawn->FindComponentByClass(TSubclassOf<UHealthComponent>()));
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
