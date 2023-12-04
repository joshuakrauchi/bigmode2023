// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayAIC.generated.h"

class UBehaviorTree;

/**
 *
 */
UCLASS()
class MODEGAME_API AGameplayAIC : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;

public:
	AGameplayAIC();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};
