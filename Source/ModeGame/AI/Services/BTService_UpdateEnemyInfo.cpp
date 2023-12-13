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

	TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) { return; }

	TObjectPtr<ABaseCharacter> BaseCharacter = AIController->GetPawn<ABaseCharacter>();
	if (!IsValid(BaseCharacter)) { return; }

	TObjectPtr<UCharacterMovementComponent> CharacterMovement = BaseCharacter->GetCharacterMovement();
	if (IsValid(CharacterMovement))
	{
		Blackboard->SetValueAsBool(OutIsFalling.SelectedKeyName, CharacterMovement->IsFalling());
	}

	Blackboard->SetValueAsBool(OutIsExhausted.SelectedKeyName, BaseCharacter->IsExhausted());
	Blackboard->SetValueAsBool(OutIsPlayingDamagedMontage.SelectedKeyName, BaseCharacter->IsPlayingDamagedMontage());

	UpdatePlayerInfo(Blackboard, BaseCharacter);
}

void UBTService_UpdateEnemyInfo::UpdatePlayerInfo(UBlackboardComponent* Blackboard, APawn* AIPawn)
{
	if (!IsValid(Blackboard)) { return; }

	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	TObjectPtr<ABaseCharacter> AICharacter = Cast<ABaseCharacter>(AIPawn);
	if (!IsValid(AICharacter)) { return; }

	TObjectPtr<ACharacter> PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!IsValid(PlayerCharacter)) { return; }

	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AICharacter);
	Params.AddIgnoredActor(AICharacter->GetFireableActor());

	World->LineTraceSingleByChannel(OutHit, AIPawn->GetActorLocation(), PlayerCharacter->GetActorLocation(), ECC_Visibility, Params);
	bool bHitPlayer = (OutHit.GetActor() == PlayerCharacter);

	Blackboard->SetValueAsObject(OutPlayerCharacter.SelectedKeyName, PlayerCharacter);
	Blackboard->SetValueAsVector(OutPlayerLocation.SelectedKeyName, PlayerCharacter->GetActorLocation());
	Blackboard->SetValueAsBool(OutHasLOSToPlayer.SelectedKeyName, bHitPlayer);
}
