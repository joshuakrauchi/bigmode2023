// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetBoolKey.generated.h"

/**
 * 
 */
UCLASS()
class MODEGAME_API UBTService_SetBoolKey : public UBTService
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector BoolKey;

	UPROPERTY(EditAnywhere)
		bool bValue;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
