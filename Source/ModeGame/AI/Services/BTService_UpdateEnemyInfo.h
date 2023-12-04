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
		FBlackboardKeySelector PlayerLocation;

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
