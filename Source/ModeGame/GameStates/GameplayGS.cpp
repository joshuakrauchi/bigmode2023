// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGS.h"
#include "Spawning/SpawnManagerComponent.h"

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

USpawnManagerComponent* AGameplayGS::GetSpawnManager() const
{
	return SpawnManagerComponent;
}

void AGameplayGS::IncreaseHealth(float Amount)
{
	CurrentHealth += FMath::Min(BaseHealth, CurrentHealth + Amount);
}

void AGameplayGS::DecreaseHealth(float Amount)
{
	CurrentHealth -= Amount;

	if (CurrentHealth <= 0.0f)
	{
		OnHealthDepleted();
	}
}

void AGameplayGS::OnHealthDepleted()
{
	UE_LOG(LogTemp, Warning, TEXT("YOU DIED"));
}
