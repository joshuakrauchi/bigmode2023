// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "GameplayPC.generated.h"

class BaseCharacter;
class UInputAction;
class UInputMappingContext;

/**
 *
 */
UCLASS()
class MODEGAME_API AGameplayPC : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TObjectPtr<UInputMappingContext> GameplayInputMappingContext = nullptr;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> LookAction = nullptr;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> FireAction = nullptr;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> GrappleAction = nullptr;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> JumpAction = nullptr;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> MeleeAction = nullptr;

	UPROPERTY()
		FVector2D MoveInput = FVector2D::Zero();

	UPROPERTY()
		bool bCanJumpAgain = false;

	UPROPERTY(EditAnywhere)
		float DoubleJumpImpulseStrength = 0.0f;

	UPROPERTY(EditAnywhere)
		float DoubleJumpHeight = 0.0f;

public:
	UFUNCTION(BlueprintCallable)
		FVector2D GetMoveInput() const;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
		void OnLookTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void OnMoveTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void OnMoveCompleted(const FInputActionValue& Value);

	UFUNCTION()
		void OnFireTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void OnFireCompleted(const FInputActionValue& Value);

	UFUNCTION()
		void OnGrappleTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void OnGrappleCompleted(const FInputActionValue& Value);

	UFUNCTION()
		void OnJumpTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void OnJumpCompleted(const FInputActionValue& Value);

	UFUNCTION()
		void OnMeleeTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void DoubleJump();

};
