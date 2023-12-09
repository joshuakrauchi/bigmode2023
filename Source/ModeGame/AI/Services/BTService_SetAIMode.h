// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AI/GameplayAIC.h"
#include "BTService_SetAIMode.generated.h"

/**
 * 
 */
UCLASS()
class MODEGAME_API UBTService_SetAIMode : public UBTService
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EAIMode AIMode = EAIMode::None;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector OutAIMode;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
