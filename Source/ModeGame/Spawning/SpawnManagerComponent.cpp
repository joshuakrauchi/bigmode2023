// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/SpawnManagerComponent.h"
#include "Spawning/Spawner.h"
#include "Components/CapsuleComponent.h"
#include "GameStates/GameplayGS.h"
#include "Spawning/PickupSpawner.h"

// Sets default values for this component's properties
USpawnManagerComponent::USpawnManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void USpawnManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentPointsPickupSpawnTime = BasePointsPickupSpawnTime;
}

// Called every frame
void USpawnManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateEnemySpawns(DeltaTime);
	UpdatePickupSpawns(DeltaTime);
}

void USpawnManagerComponent::RegisterSpawner(ASpawner* Spawner)
{
	Spawners.Add(Spawner);
}

void USpawnManagerComponent::RegisterPickupSpawner(APickupSpawner* Spawner)
{
	PickupSpawners.Add(Spawner);
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
		
		bool bReplaceBest = (CurrentSeconds > SecondsSinceLastSpawn);
		if ((CurrentSeconds == SecondsSinceLastSpawn) && FMath::RandBool())
		{
			bReplaceBest = true;
		}

		if (bReplaceBest)
		{
			LeastRecentlyUsedSpawner = Spawner;
			SecondsSinceLastSpawn = CurrentSeconds;
		}
	}

	return LeastRecentlyUsedSpawner;
}

APickupSpawner* USpawnManagerComponent::DetermineBestPickupSpawner() const
{
	if (Spawners.Num() <= 0) { return nullptr; }

	TObjectPtr<APickupSpawner> LeastRecentlyUsedSpawner = nullptr;
	float SecondsSinceLastSpawn = 0.0f;

	for (const TObjectPtr<APickupSpawner> Spawner : PickupSpawners)
	{
		if (!IsValid(Spawner)) { continue; }
		if (Spawner->HasSpawnedPickup()) { continue; }
		float CurrentSeconds = Spawner->GetSecondsSinceLastSpawn();

		bool bReplaceBest = (CurrentSeconds > SecondsSinceLastSpawn);
		if ((CurrentSeconds == SecondsSinceLastSpawn) && FMath::RandBool())
		{
			bReplaceBest = true;
		}

		if (bReplaceBest)
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

void USpawnManagerComponent::UpdateEnemySpawns(float DeltaSeconds)
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	TObjectPtr<AGameplayGS> GameplayGS = World->GetGameState<AGameplayGS>();
	if (!IsValid(GameplayGS)) { return; }

	CurrentSpawnTime -= DeltaSeconds;

	int NumCharacters = GameplayGS->GetNumCharacters();
	if ((NumCharacters >= MinCharactersSpawned) && ((NumCharacters >= MaxCharactersSpawned) || CurrentSpawnTime > 0.0f)) { return; }

	if (PossibleSpawns.Num() <= 0) { return; }

	AActor* SpawnedActor = nullptr;
	bool bSpawnSucceeded = TrySpawnActor(PossibleSpawns[FMath::RandRange(0, PossibleSpawns.Num() - 1)], SpawnedActor);

	if (bSpawnSucceeded)
	{
		ResetSpawnTime();
	}
}

void USpawnManagerComponent::UpdatePickupSpawns(float DeltaSeconds)
{
	if (IsValid(SpawnedScorePickup)) { return; }

	CurrentPointsPickupSpawnTime -= DeltaSeconds;
	if (CurrentPointsPickupSpawnTime > 0.0f) { return; }

	if (!IsValid(PointsPickupClass)) { return; }

	TObjectPtr<APickupSpawner> PickupSpawner = DetermineBestPickupSpawner();
	if (!IsValid(PickupSpawner)) { return; }

	AActor* SpawnedActor = nullptr;
	bool bSpawnSucceeded = PickupSpawner->TrySpawnActor(PointsPickupClass, SpawnedActor);

	if (bSpawnSucceeded)
	{
		SpawnedScorePickup = SpawnedActor;
		CurrentPointsPickupSpawnTime = BasePointsPickupSpawnTime;
	}
}

bool USpawnManagerComponent::TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor)
{
	TObjectPtr<ASpawner> BestSpawner = DetermineBestSpawner();
	if (!IsValid(BestSpawner)) { return false; }

	bool bSpawnSucceeded = BestSpawner->TrySpawnActor(ActorClass, OutSpawnedActor);

	return bSpawnSucceeded;
}

void USpawnManagerComponent::ResetSpawnTime()
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	TObjectPtr<AGameplayGS> GameplayGS = World->GetGameState<AGameplayGS>();
	if (!IsValid(GameplayGS)) { return; }

	CurrentSpawnTime = (BaseSpawnTime * FMath::Pow(SpawnMultiplierBaseToPowOfNumCharacters, GameplayGS->GetNumCharacters()));
}
