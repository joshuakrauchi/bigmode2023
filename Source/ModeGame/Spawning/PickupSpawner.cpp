// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawning/PickupSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "GameStates/GameplayGS.h"
#include "Spawning/SpawnManagerComponent.h"

// Sets default values
APickupSpawner::APickupSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EmptyRoot = CreateDefaultSubobject<USceneComponent>(FName("EmptyRoot"));
	SetRootComponent(EmptyRoot);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(FName("SpawnPoint"));
	SpawnPoint->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APickupSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	TryRegisterWithSpawnManager();
}

bool APickupSpawner::TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor)
{
	if (!IsValid(ActorClass)) { return false; }
	if (!IsValid(SpawnPoint)) { return false; }

	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return false; }

	OutSpawnedActor = World->SpawnActor<AActor>(
		ActorClass,
		SpawnPoint->GetComponentLocation(),
		SpawnPoint->GetComponentRotation()
		);

	bool bSpawnSucceeded = IsValid(OutSpawnedActor);

	if (bSpawnSucceeded)
	{
		SpawnedPickup = OutSpawnedActor;
		TimeAtLastSpawn = UGameplayStatics::GetTimeSeconds(World);
	}

	return bSpawnSucceeded;
}

float APickupSpawner::GetSecondsSinceLastSpawn() const
{
	return (UGameplayStatics::GetTimeSeconds(GetWorld()) - TimeAtLastSpawn);
}

bool APickupSpawner::HasSpawnedPickup() const
{
	return IsValid(SpawnedPickup);
}

bool APickupSpawner::TryRegisterWithSpawnManager()
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return false; }

	TObjectPtr<AGameplayGS> GameplayGS = World->GetGameState<AGameplayGS>();
	if (!IsValid(GameplayGS)) { return false; }

	TObjectPtr<USpawnManagerComponent> SpawnManager = GameplayGS->GetSpawnManager();
	if (!IsValid(SpawnManager)) { return false; }

	SpawnManager->RegisterPickupSpawner(this);

	return true;
}

