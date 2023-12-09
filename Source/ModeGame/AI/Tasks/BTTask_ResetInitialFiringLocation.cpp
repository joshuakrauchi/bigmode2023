// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_ResetInitialFiringLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/GameplayAIC.h"

EBTNodeResult::Type UBTTask_ResetInitialFiringLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<AGameplayAIC> GameplayAIC = Cast<AGameplayAIC>(OwnerComp.GetAIOwner());
	if (!IsValid(GameplayAIC)) { return EBTNodeResult::Failed; }

	GameplayAIC->ResetInitialFiringRotation();

	return EBTNodeResult::Succeeded;
}
