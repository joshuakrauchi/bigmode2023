// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPC.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "../Characters/BaseCharacter.h"

void AGameplayPC::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(EnhancedInputSubsystem)) { return; }

	EnhancedInputSubsystem->AddMappingContext(GameplayInputMappingContext, 0);

	TObjectPtr<ABaseCharacter> BaseCharacter = GetPawn<ABaseCharacter>();
	if (IsValid(BaseCharacter))
	{
		BaseCharacter->SetFirstPersonMode(true);
	}
}

void AGameplayPC::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent)) { return; }

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameplayPC::OnLookTriggered);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameplayPC::OnMoveTriggered);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AGameplayPC::OnFireTriggered);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AGameplayPC::OnFireCompleted);
	EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Triggered, this, &AGameplayPC::OnGrappleTriggered);
	EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Completed, this, &AGameplayPC::OnGrappleCompleted);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGameplayPC::OnJumpTriggered);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGameplayPC::OnJumpCompleted);
	EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &AGameplayPC::OnMeleeTriggered);
}

void AGameplayPC::OnLookTriggered(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	
	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void AGameplayPC::OnMoveTriggered(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	TObjectPtr<APawn> PossessedPawn = GetPawn();
	if (!IsValid(PossessedPawn)) { return; }

	PossessedPawn->AddMovementInput(PossessedPawn->GetActorRightVector(), MoveVector.X);
	PossessedPawn->AddMovementInput(PossessedPawn->GetActorForwardVector(), MoveVector.Y);
}

void AGameplayPC::OnFireTriggered(const FInputActionValue& Value)
{
	
}

void AGameplayPC::OnFireCompleted(const FInputActionValue& Value)
{

}

void AGameplayPC::OnGrappleTriggered(const FInputActionValue& Value)
{

}

void AGameplayPC::OnGrappleCompleted(const FInputActionValue& Value)
{

}

void AGameplayPC::OnJumpTriggered(const FInputActionValue& Value)
{
	TObjectPtr<ACharacter> PossessedCharacter = GetCharacter();
	if (!IsValid(PossessedCharacter)) { return; }

	PossessedCharacter->Jump();
}

void AGameplayPC::OnJumpCompleted(const FInputActionValue& Value)
{
	TObjectPtr<ACharacter> PossessedCharacter = GetCharacter();
	if (!IsValid(PossessedCharacter)) { return; }

	PossessedCharacter->StopJumping();
}

void AGameplayPC::OnMeleeTriggered(const FInputActionValue& Value)
{

}

