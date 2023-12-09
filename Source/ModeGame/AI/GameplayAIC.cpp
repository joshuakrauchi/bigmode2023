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

	if (AIPawn->GetCharacterMovement()->IsFalling())
	{
		FVector v = (GetBlackboardComponent()->GetValueAsVector(FName("MoveLocation")) - AIPawn->GetActorLocation());
		v = v.GetClampedToMaxSize(200);
		v *= 300 * DeltaTime;
		v.Z = AIPawn->GetCharacterMovement()->Velocity.Z;

		AIPawn->GetCharacterMovement()->Velocity = (v);
	}



	/*FHitResult OutResult;
	FVector End = (AIPawn->GetActorLocation()) + AIPawn->GetActorForwardVector() * 100.0f;
	GetWorld()->LineTraceSingleByChannel(OutResult, AIPawn->GetActorLocation(), End, ECC_Visibility);

	if (OutResult.bBlockingHit)
	{
		if (AIPawn->CanJump())
		{
			AIPawn->Jump();
		}
	}*/

	UpdateFiringRotation(DeltaTime);
	UpdateFocus();
	UpdateFiring();
}

void AGameplayAIC::UpdateFocus()
{
	TObjectPtr<APawn> PossessedPawn = GetPawn();
	if (!IsValid(PossessedPawn)) { return; }

	if (AIMode != EAIMode::Firing) { return; }

	PossessedPawn->SetActorRotation(FRotator(0.0f, CurrentFiringRotation.Yaw, 0.0f));
	SetControlRotation(CurrentFiringRotation);
}

void AGameplayAIC::UpdateFiringRotation(float DeltaSeconds)
{
	if (!IsValid(TargetController)) { return; }
	TObjectPtr<APawn> TargetPawn = TargetController->GetPawn();
	if (!IsValid(TargetPawn)) { return; }

	TObjectPtr<APawn> PossessedPawn = GetPawn();
	if (!IsValid(PossessedPawn)) { return; }

	float RotationSpeed = (AccuracyCorrectionMoveSpeed * DeltaSeconds);
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PossessedPawn->GetActorLocation(), TargetPawn->GetActorLocation());
	
	FRotator RotationDifference = UKismetMathLibrary::NormalizedDeltaRotator(CurrentFiringRotation, TargetRotation);

	FQuat RotatedQuat = FQuat::Slerp(FQuat(CurrentFiringRotation), FQuat(TargetRotation), RotationSpeed);
	CurrentFiringRotation = FRotator(RotatedQuat);
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
