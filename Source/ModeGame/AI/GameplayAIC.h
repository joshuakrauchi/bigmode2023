// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Characters/BaseCharacter.h"
#include "GameplayAIC.generated.h"

class UBehaviorTree;
class ABaseCharacter;
class AGameplayPC;

UENUM()
enum class EAIMode : uint8
{
	None,

	Moving,
	Firing,
	Exhausted,

	Count,
};

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

	UPROPERTY(EditAnywhere)
		float InitialFiringRotationOffset = 0.0f;

	UPROPERTY(EditAnywhere)
		float AccuracyCorrectionMoveSpeed = 0.0f;

private:
	UPROPERTY()
		TObjectPtr<AGameplayPC> TargetController = nullptr;

	UPROPERTY()
		FRotator CurrentFiringRotation;

	UPROPERTY()
		EAIMode AIMode = EAIMode::None;

public:
	AGameplayAIC();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn /* = true */) override;

	void ResetInitialFiringRotation();

	void SetAIMode(EAIMode NewAIMode);

private:
	void UpdateFocus();

	void UpdateFiringRotation(float DeltaSeconds);

	void UpdateFiring();

};
