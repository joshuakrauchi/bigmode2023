// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_SetBoolKey.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_SetBoolKey::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(Blackboard)) { return; }

	Blackboard->SetValueAsBool(BoolKey.SelectedKeyName, bValue);
}
