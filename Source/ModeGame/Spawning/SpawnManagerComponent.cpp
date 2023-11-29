// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/SpawnManagerComponent.h"
#include "SpawnerComponent.h"

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
		TimeBeforeSpawnByClass.Add(SpawnInfo.ActorClass, SpawnInfo.BaseTimeBeforeSpawn);
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
				TimeBeforeSpawn = (SpawnInfo.BaseTimeBeforeSpawn * CurrentSpawnTimeMultiplier);
			}
		}
	}
}

void USpawnManagerComponent::RegisterSpawnerComponent(USpawnerComponent* SpawnerComponent)
{
	SpawnerComponents.Add(SpawnerComponent);
}

USpawnerComponent* USpawnManagerComponent::DetermineBestSpawnerComponent()
{
	if (SpawnerComponents.Num() <= 0) { return nullptr; }

	TObjectPtr<USpawnerComponent> LeastRecentlyUsedSpawnComponent = nullptr;
	float SecondsSinceLastSpawn = 0.0f;

	for (const TObjectPtr<USpawnerComponent> SpawnerComponent : SpawnerComponents)
	{
		if (!IsValid(SpawnerComponent)) { continue; }
		float CurrentSeconds = SpawnerComponent->GetSecondsSinceLastSpawn();
		if (CurrentSeconds > SecondsSinceLastSpawn)
		{
			LeastRecentlyUsedSpawnComponent = SpawnerComponent;
			SecondsSinceLastSpawn = CurrentSeconds;
		}
	}

	return LeastRecentlyUsedSpawnComponent;
}

bool USpawnManagerComponent::TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor)
{
	TObjectPtr<USpawnerComponent> BestSpawnerComponent = DetermineBestSpawnerComponent();
	if (!IsValid(BestSpawnerComponent)) { return false; }

	bool bSpawnSucceeded = BestSpawnerComponent->TrySpawnActor(ActorClass, OutSpawnedActor);

	return bSpawnSucceeded;
}
