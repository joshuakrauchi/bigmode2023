// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPC.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/Fireable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

FVector2D AGameplayPC::GetMoveInput() const
{
	return MoveInput;
}

void AGameplayPC::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(EnhancedInputSubsystem)) { return; }

	EnhancedInputSubsystem->AddMappingContext(GameplayInputMappingContext, 0);
}

void AGameplayPC::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent)) { return; }

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameplayPC::OnLookTriggered);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameplayPC::OnMoveTriggered);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AGameplayPC::OnMoveTriggered);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AGameplayPC::OnFireTriggered);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AGameplayPC::OnFireCompleted);
	EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Started, this, &AGameplayPC::OnGrappleTriggered);
	EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Completed, this, &AGameplayPC::OnGrappleCompleted);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGameplayPC::OnJumpTriggered);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGameplayPC::OnJumpCompleted);
	EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AGameplayPC::OnMeleeTriggered);
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

	MoveInput = MoveVector;

	TObjectPtr<APawn> PossessedPawn = GetPawn();
	if (!IsValid(PossessedPawn)) { return; }

	PossessedPawn->AddMovementInput(PossessedPawn->GetActorRightVector(), MoveVector.X);
	PossessedPawn->AddMovementInput(PossessedPawn->GetActorForwardVector(), MoveVector.Y);
}

void AGameplayPC::OnMoveCompleted(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	MoveInput = MoveVector;
}

void AGameplayPC::OnFireTriggered(const FInputActionValue& Value)
{
	TObjectPtr<ABaseCharacter> BaseCharacter = GetPawn<ABaseCharacter>();
	if (!IsValid(BaseCharacter)) { return; }

	IFireable* Fireable = Cast<IFireable>(BaseCharacter->GetFireableActor());
	if (Fireable == nullptr) { return; }

	Fireable->TryBeginFire();
}

void AGameplayPC::OnFireCompleted(const FInputActionValue& Value)
{
	TObjectPtr<ABaseCharacter> BaseCharacter = GetPawn<ABaseCharacter>();
	if (!IsValid(BaseCharacter)) { return; }

	IFireable* Fireable = Cast<IFireable>(BaseCharacter->GetFireableActor());
	if (Fireable == nullptr) { return; }

	Fireable->TryEndFire();
}

void AGameplayPC::OnGrappleTriggered(const FInputActionValue& Value)
{

}

void AGameplayPC::OnGrappleCompleted(const FInputActionValue& Value)
{

}

void AGameplayPC::OnJumpTriggered(const FInputActionValue& Value)
{
	TObjectPtr<ABaseCharacter> PossessedCharacter = GetPawn<ABaseCharacter>();
	if (!IsValid(PossessedCharacter)) { return; }

	if (PossessedCharacter->CanJump())
	{
		PossessedCharacter->Jump();
		PossessedCharacter->ResetDoubleJump();
	}
	else if (PossessedCharacter->CanDoubleJump())
	{
		PossessedCharacter->DoubleJump();
	}
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
