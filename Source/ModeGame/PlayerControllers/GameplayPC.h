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
		bool bIsCameraRotationDisabled = false;

public:
	UFUNCTION(BlueprintCallable)
		FVector2D GetMoveInput() const;

	UFUNCTION(BlueprintCallable)
		void SetCameraRotationEnabled(bool bEnabled);

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
		void OnFireStarted(const FInputActionValue& Value);

	UFUNCTION()
		void OnFireCompleted(const FInputActionValue& Value);

	UFUNCTION()
		void OnGrappleStarted(const FInputActionValue& Value);

	UFUNCTION()
		void OnGrappleCompleted(const FInputActionValue& Value);

	UFUNCTION()
		void OnJumpStarted(const FInputActionValue& Value);

	UFUNCTION()
		void OnJumpCompleted(const FInputActionValue& Value);

	UFUNCTION()
		void OnMeleeStarted(const FInputActionValue& Value);

};
