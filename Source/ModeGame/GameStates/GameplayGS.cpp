// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGS.h"
#include "Spawning/SpawnManagerComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AGameplayGS::AGameplayGS()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnManagerComponent = CreateDefaultSubobject<USpawnManagerComponent>(FName("SpawnManagerComponent"));
}

void AGameplayGS::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = BaseHealth;
}

void AGameplayGS::Tick(float DeltaSeconds)
{
	UpdateComboEndTime(DeltaSeconds);
}

USpawnManagerComponent* AGameplayGS::GetSpawnManager() const
{
	return SpawnManagerComponent;
}

void AGameplayGS::IncreaseHealth(float Amount)
{
	CurrentHealth = FMath::Min(BaseHealth, CurrentHealth + Amount);
}

void AGameplayGS::DecreaseHealth(float Amount)
{
	bool bAlreadyDead = (CurrentHealth <= 0.0f);
	CurrentHealth -= Amount;

	if (!bAlreadyDead && (CurrentHealth <= 0.0f))
	{
		OnHealthDepleted();
	}
}

int AGameplayGS::GetScore()
{
	return CurrentScore;
}

void AGameplayGS::AddScore(int ScoreAmount)
{
	if (ScoreAmount <= 0) { return; }

	CurrentScore += (ScoreAmount * (1.0f + (ScoreMultiplierIncreasePerComboKill * CurrentComboCount)));
}

void AGameplayGS::RegisterCharacter(ABaseCharacter* Character)
{
	AllCharacters.Add(Character);
}

void AGameplayGS::DeregisterCharacter(ABaseCharacter* Character)
{
	AllCharacters.Remove(Character);
}

int AGameplayGS::GetNumCharacters() const
{
	return AllCharacters.Num();
}

void AGameplayGS::IncrementComboCount()
{
	++CurrentComboCount;

	CurrentTimeUntilComboEnd = BaseTimeUntilComboEnd;
}

void AGameplayGS::ClearComboCount()
{
	CurrentComboCount = 0;
}

void AGameplayGS::UpdateComboEndTime(float DeltaSeconds)
{
	CurrentTimeUntilComboEnd -= DeltaSeconds;

	if (CurrentTimeUntilComboEnd <= 0.0f)
	{
		ClearComboCount();
	}
}
