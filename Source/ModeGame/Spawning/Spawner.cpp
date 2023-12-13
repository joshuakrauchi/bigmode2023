// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/Spawner.h"
#include "Kismet/GameplayStatics.h"
#include "GameStates/GameplayGS.h"
#include "SpawnManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("SpawnCapsule"));
	SetRootComponent(SpawnCapsule);

	SpawnParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("SpawnParticleComponent"));
	SpawnParticleComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	bool bRegisterSucceeded = TryRegisterWithSpawnManager();
}

bool ASpawner::TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor)
{
	OutSpawnedActor = nullptr;

	if (!IsValid(ActorClass)) { return false; }
	if (!IsValid(SpawnCapsule)) { return false; }
	if (!IsValid(SpawnParticleComponent)) { return false; }

	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return false; }

	OutSpawnedActor = World->SpawnActor<AActor>(
		ActorClass,
		SpawnCapsule->GetComponentLocation(),
		SpawnCapsule->GetComponentRotation()
		);

	bool bSpawnSucceeded = IsValid(OutSpawnedActor);

	if (bSpawnSucceeded)
	{
		TimeAtLastSpawn = UGameplayStatics::GetTimeSeconds(World);
		SpawnParticleComponent->Activate(true);
	}

	return bSpawnSucceeded;
}

float ASpawner::GetSecondsSinceLastSpawn() const
{
	return (UGameplayStatics::GetTimeSeconds(GetWorld()) - TimeAtLastSpawn);
}

UCapsuleComponent* ASpawner::GetCapsule() const
{
	return SpawnCapsule;
}

bool ASpawner::TryRegisterWithSpawnManager()
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return false; }
	
	TObjectPtr<AGameplayGS> GameplayGS = World->GetGameState<AGameplayGS>();
	if (!IsValid(GameplayGS)) { return false; }

	TObjectPtr<USpawnManagerComponent> SpawnManager = GameplayGS->GetSpawnManager();
	if (!IsValid(SpawnManager)) { return false; }

	SpawnManager->RegisterSpawner(this);

	return true;
}
