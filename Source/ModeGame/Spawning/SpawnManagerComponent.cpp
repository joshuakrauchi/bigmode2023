// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/SpawnManagerComponent.h"
#include "Spawning/Spawner.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
USpawnManagerComponent::USpawnManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentSpawnTimeMultiplier = BaseSpawnTimeMultiplier;
}

// Called when the game starts
void USpawnManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const FSpawnInfo& SpawnInfo : SpawnInfos)
	{
		TimeBeforeSpawnByClass.Add(SpawnInfo.ActorClass, SpawnInfo.InitialTimeBeforeSpawn);
	}
}

// Called every frame
void USpawnManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (const FSpawnInfo& SpawnInfo : SpawnInfos)
	{
		float& TimeBeforeSpawn = TimeBeforeSpawnByClass[SpawnInfo.ActorClass];

		TimeBeforeSpawn -= DeltaTime;
		if (TimeBeforeSpawn < 0.0f)
		{
			AActor* SpawnedActor = nullptr;
			bool bSpawnSucceeded = TrySpawnActor(SpawnInfo.ActorClass, SpawnedActor);

			if (bSpawnSucceeded)
			{
				TimeBeforeSpawn = (SpawnInfo.BaseTimeBetweenSpawns * CurrentSpawnTimeMultiplier);
			}
		}
	}
}

void USpawnManagerComponent::RegisterSpawner(ASpawner* Spawner)
{
	Spawners.Add(Spawner);
}

ASpawner* USpawnManagerComponent::DetermineBestSpawner()
{
	if (Spawners.Num() <= 0) { return nullptr; }

	TObjectPtr<ASpawner> LeastRecentlyUsedSpawner = nullptr;
	float SecondsSinceLastSpawn = 0.0f;

	for (const TObjectPtr<ASpawner> Spawner : Spawners)
	{
		if (!IsValid(Spawner)) { continue; }
		float CurrentSeconds = Spawner->GetSecondsSinceLastSpawn();
		if (CurrentSeconds > SecondsSinceLastSpawn)
		{
			LeastRecentlyUsedSpawner = Spawner;
			SecondsSinceLastSpawn = CurrentSeconds;
		}
	}

	return LeastRecentlyUsedSpawner;
}

ASpawner* USpawnManagerComponent::GetClosestSpawnerToLocation(FVector Location) const
{
	TObjectPtr<ASpawner> ClosestSpawner = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	for (TObjectPtr<ASpawner> Spawner : Spawners)
	{
		if (!IsValid(Spawner)) { continue; }

		TObjectPtr<UCapsuleComponent> Capsule = Spawner->GetCapsule();
		if (!IsValid(Capsule)) { continue; }
		
		float CurrentDistance = FVector::DistSquared(Location, Capsule->GetComponentLocation());
		if (CurrentDistance <= ClosestDistance)
		{
			ClosestDistance = CurrentDistance;
			ClosestSpawner = Spawner;
		}
	}

	return ClosestSpawner;
}

bool USpawnManagerComponent::TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor)
{
	TObjectPtr<ASpawner> BestSpawner = DetermineBestSpawner();
	if (!IsValid(BestSpawner)) { return false; }

	bool bSpawnSucceeded = BestSpawner->TrySpawnActor(ActorClass, OutSpawnedActor);

	return bSpawnSucceeded;
}
