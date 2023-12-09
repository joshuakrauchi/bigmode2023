// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTService_UpdateEnemyInfo.generated.h"

/**
 * 
 */
UCLASS()
class MODEGAME_API UBTService_UpdateEnemyInfo : public UBTService
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector OutPlayerLocation;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector OutIsFalling;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector OutPlayerCharacter;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector OutIsExhausted;

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UFUNCTION()
		void UpdatePlayerInfo(UBlackboardComponent* Blackboard, APawn* AIPawn);

};
