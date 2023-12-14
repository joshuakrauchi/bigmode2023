// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_UpdateEnemyInfo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBTService_UpdateEnemyInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(Blackboard)) { return; }

	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) { return; }

	TObjectPtr<ABaseCharacter> AICharacter = AIController->GetPawn<ABaseCharacter>();
	if (!IsValid(AICharacter)) { return; }

	TObjectPtr<ACharacter> PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!IsValid(PlayerCharacter)) { return; }

	Blackboard->SetValueAsBool(OutIsExhausted.SelectedKeyName, AICharacter->IsExhausted());
	Blackboard->SetValueAsBool(OutIsPlayingDamagedMontage.SelectedKeyName, AICharacter->IsPlayingDamagedMontage());

	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AICharacter);
	Params.AddIgnoredActor(AICharacter->GetFireableActor());

	World->LineTraceSingleByChannel(OutHit, AICharacter->GetActorLocation(), PlayerCharacter->GetActorLocation(), ECC_Visibility, Params);
	bool bHitPlayer = (OutHit.GetActor() == PlayerCharacter);

	Blackboard->SetValueAsBool(OutHasLOSToPlayer.SelectedKeyName, bHitPlayer);
}
