// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAIC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/Fireable.h"
#include "PlayerControllers/GameplayPC.h"

AGameplayAIC::AGameplayAIC()
{
	PrimaryActorTick.bCanEverTick = true;

	bSetControlRotationFromPawnOrientation = false;
}

void AGameplayAIC::BeginPlay()
{
	Super::BeginPlay();

	TargetController = Cast<AGameplayPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void AGameplayAIC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* AIPawn = GetPawn<ACharacter>();
	if (!IsValid(AIPawn)) { return; }

	TObjectPtr<ACharacter> PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!IsValid(PlayerCharacter)) { return; }

	return;
	UpdateFiringRotation(DeltaTime);
	UpdateFocus();
	UpdateFiring();
}

void AGameplayAIC::UpdateControlRotation(float DeltaTime, bool bUpdatePawn /* = true */)
{
	// Copied from AAIController except commented some code that prevented pitch from being affected.
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		FRotator NewControlRotation = GetControlRotation();

		// Look toward focus
		const FVector FocalPoint = GetFocalPoint();
		if (FAISystem::IsValidLocation(FocalPoint))
		{
			NewControlRotation = (FocalPoint - MyPawn->GetPawnViewLocation()).Rotation();
		}
		else if (bSetControlRotationFromPawnOrientation)
		{
			NewControlRotation = MyPawn->GetActorRotation();
		}

		// Don't pitch view unless looking at another pawn
		/*if (NewControlRotation.Pitch != 0 && Cast<APawn>(GetFocusActor()) == nullptr)
		{
			NewControlRotation.Pitch = 0.f;
		}*/

		SetControlRotation(NewControlRotation);

		if (bUpdatePawn)
		{
			const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

			if (CurrentPawnRotation.Equals(NewControlRotation, 1e-3f) == false)
			{
				MyPawn->FaceRotation(NewControlRotation, DeltaTime);
			}
		}
	}
}

void AGameplayAIC::UpdateFocus()
{
	TObjectPtr<APawn> PossessedPawn = GetPawn();
	if (!IsValid(PossessedPawn)) { return; }

	if (AIMode != EAIMode::Firing) { return; }

	SetControlRotation(CurrentFiringRotation);
	
}

void AGameplayAIC::UpdateFiringRotation(float DeltaSeconds)
{
	if (!IsValid(TargetController)) { return; }
	TObjectPtr<APawn> TargetPawn = TargetController->GetPawn();
	if (!IsValid(TargetPawn)) { return; }

	TObjectPtr<APawn> PossessedPawn = GetPawn();
	if (!IsValid(PossessedPawn)) { return; }

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PossessedPawn->GetActorLocation(), TargetPawn->GetActorLocation());
	CurrentFiringRotation = UKismetMathLibrary::RInterpTo(CurrentFiringRotation, TargetRotation, DeltaSeconds, AccuracyCorrectionMoveSpeed);
}

void AGameplayAIC::UpdateFiring()
{
	TObjectPtr<ABaseCharacter> BaseCharacter = GetPawn<ABaseCharacter>();
	if (!IsValid(BaseCharacter)) { return; }

	IFireable* Fireable = Cast<IFireable>(BaseCharacter->GetFireableActor());
	if (Fireable == nullptr) { return; }

	if (AIMode == EAIMode::Firing)
	{
		Fireable->TryBeginFire();
	}
	else
	{
		Fireable->TryEndFire();
	}
}

void AGameplayAIC::ResetInitialFiringRotation()
{
	if (!IsValid(TargetController)) { return; }
	TObjectPtr<APawn> TargetPawn = TargetController->GetPawn();
	if (!IsValid(TargetPawn)) { return; }

	TObjectPtr<APawn> PossessedPawn = GetPawn();
	if (!IsValid(PossessedPawn)) { return; }

	float Offset = (InitialFiringRotationOffset / 2.0f);
	FRotator OffsetRotation = FRotator(FMath::RandRange(0.0f, Offset), FMath::RandRange(-Offset, Offset), 0.0f);

	FVector FiringLocation = OffsetRotation.RotateVector(TargetPawn->GetActorLocation());

	CurrentFiringRotation = UKismetMathLibrary::FindLookAtRotation(PossessedPawn->GetActorLocation(), FiringLocation);
}

void AGameplayAIC::SetAIMode(EAIMode NewAIMode)
{
	AIMode = NewAIMode;
}
