// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/EnemyQueryContext.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


void UEnemyQueryContext::ProvideContext(
    FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	UBlackboardComponent* Blackboard = 
            UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!Blackboard) return;

    UObject* ContextActor = Blackboard->GetValueAsObject(EnemyActorKeyName);

	UEnvQueryItemType_Actor::SetContextHelper(
        ContextData, Cast<AActor>(ContextActor));
}
