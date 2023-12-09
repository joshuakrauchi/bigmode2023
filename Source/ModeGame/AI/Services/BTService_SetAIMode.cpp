// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_SetAIMode.h"
#include "AI/GameplayAIC.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_SetAIMode::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(Blackboard)) { return; }

	TObjectPtr<AGameplayAIC> GameplayAIC = Cast<AGameplayAIC>(OwnerComp.GetAIOwner());
	if (!IsValid(GameplayAIC)) { return; }

	GameplayAIC->SetAIMode(AIMode);
	Blackboard->SetValueAsEnum(OutAIMode.SelectedKeyName, StaticCast<uint8>(AIMode));
}
