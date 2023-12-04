// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/SpawnerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameStates/GameplayGS.h"
#include "SpawnManagerComponent.h"

// Sets default values for this component's properties
USpawnerComponent::USpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	bool bRegisterSucceeded = TryRegisterWithSpawnManager();
}

bool USpawnerComponent::TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor)
{
	OutSpawnedActor = nullptr;

	if (!IsValid(ActorClass)) { return false; }

	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return false; }

	OutSpawnedActor = World->SpawnActor<AActor>(
		ActorClass,
		FTransform(GetComponentLocation())
		);

	bool bSpawnSucceeded = IsValid(OutSpawnedActor);

	if (bSpawnSucceeded)
	{
		TimeAtLastSpawn = UGameplayStatics::GetTimeSeconds(World);
	}

	return bSpawnSucceeded;
}

float USpawnerComponent::GetSecondsSinceLastSpawn() const
{
	return (UGameplayStatics::GetTimeSeconds(GetWorld()) - TimeAtLastSpawn);
}

bool USpawnerComponent::TryRegisterWithSpawnManager()
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return false; }
	
	TObjectPtr<AGameplayGS> GameplayGS = World->GetGameState<AGameplayGS>();
	if (!IsValid(GameplayGS)) { return false; }

	TObjectPtr<USpawnManagerComponent> SpawnManager = GameplayGS->GetSpawnManager();
	if (!IsValid(SpawnManager)) { return false; }

	SpawnManager->RegisterSpawnerComponent(this);

	return true;
}
