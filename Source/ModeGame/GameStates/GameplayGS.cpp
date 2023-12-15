// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGS.h"
#include "Spawning/SpawnManagerComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/MainGameInstance.h"

AGameplayGS::AGameplayGS()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnManagerComponent = CreateDefaultSubobject<USpawnManagerComponent>(FName("SpawnManagerComponent"));
}

void AGameplayGS::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = BaseHealth;

	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	LevelDirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(World, ADirectionalLight::StaticClass()));
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

	if (!IsValid(LevelDirectionalLight)) { return; }

	float NormalisedScore = FMath::Clamp((float) CurrentScore / DuskScoreAmount, 0, 1);
	float SunRotation = FMath::Lerp(StartingSunRotation, DuskSunRotation, NormalisedScore);

	FRotator LightRotation{ CurrentScore < NightScoreAmount ? SunRotation : NightSunRotation, 0.f, 0.f };

	LevelDirectionalLight->SetActorRelativeRotation(LightRotation);
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
	OnComboIncreased();
	CurrentTimeUntilComboEnd = BaseTimeUntilComboEnd;
}

void AGameplayGS::ClearComboCount()
{
	CurrentComboCount = 0;
}

float AGameplayGS::GetBaseHealth() const
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return 0.0f; }

	TObjectPtr<UMainGameInstance> GameInstance = World->GetGameInstance<UMainGameInstance>();
	if (!IsValid(GameInstance)) { return 0.0f; }

	return (GameInstance->IsHardMode() ? HardBaseHealth : BaseHealth);
}

void AGameplayGS::UpdateComboEndTime(float DeltaSeconds)
{
	CurrentTimeUntilComboEnd -= DeltaSeconds;

	if (CurrentTimeUntilComboEnd <= 0.0f)
	{
		ClearComboCount();
	}
}
