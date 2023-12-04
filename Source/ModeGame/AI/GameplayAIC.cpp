// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAIC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AGameplayAIC::AGameplayAIC()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameplayAIC::BeginPlay()
{
	Super::BeginPlay();

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

	if (false && AIPawn->GetCharacterMovement()->IsFalling())
	{


		FVector v = (PlayerCharacter->GetActorLocation() - AIPawn->GetActorLocation());
		v.Normalize();
		v.Z = 0;
		StopMovement();
		AIPawn->GetCharacterMovement()->StopMovementImmediately();
		AIPawn->SetActorLocation(AIPawn->GetActorLocation());
		AIPawn->GetCharacterMovement()->AddImpulse(v * 5000 * DeltaTime, true);

		UE_LOG(LogTemp, Warning, TEXT("I'M FLYING!!!"));
	}

	FHitResult OutResult;
	FVector End = (AIPawn->GetActorLocation()) + AIPawn->GetActorForwardVector() * 100.0f;
	GetWorld()->LineTraceSingleByChannel(OutResult, AIPawn->GetActorLocation(), End, ECC_Visibility);

	if (OutResult.bBlockingHit)
	{
		if (AIPawn->CanJump())
		{
			AIPawn->Jump();
		}
	}

}
