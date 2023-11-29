// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_UpdateEnemyInfo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void UBTService_UpdateEnemyInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();

	TObjectPtr<ACharacter> PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (IsValid(PlayerCharacter))
	{
		Blackboard->SetValueAsVector(PlayerLocation.SelectedKeyName, PlayerCharacter->GetActorLocation());
	}
}
