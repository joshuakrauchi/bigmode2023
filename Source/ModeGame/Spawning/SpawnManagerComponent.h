// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnManagerComponent.generated.h"

class ASpawner;
class APickupSpawner;

UCLASS()
class MODEGAME_API USpawnManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TArray<TObjectPtr<UClass>> PossibleSpawns;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UClass> PointsPickupClass = nullptr;

	UPROPERTY(EditAnywhere)
		float BasePointsPickupSpawnTime = 0.0f;

	UPROPERTY(EditAnywhere)
		float CurrentPointsPickupSpawnTime = 0.0f;

	UPROPERTY(EditAnywhere)
		float SpawnMultiplierBaseToPowOfNumCharacters = 1.0f;

	UPROPERTY(EditAnywhere)
		float BaseSpawnTime = 1.0f;

	UPROPERTY()
		float CurrentSpawnTime = 0.0f;

	UPROPERTY()
		TArray<TObjectPtr<ASpawner>> Spawners;

	UPROPERTY()
		TArray<TObjectPtr<APickupSpawner>> PickupSpawners;

	UPROPERTY(EditAnywhere)
		int MinCharactersSpawned = 0;

	UPROPERTY(EditAnywhere)
		int MaxCharactersSpawned = 0;

	UPROPERTY()
		TObjectPtr<AActor> SpawnedScorePickup = nullptr;

public:
	// Sets default values for this component's properties
	USpawnManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void RegisterSpawner(ASpawner* Spawner);

	UFUNCTION()
		void RegisterPickupSpawner(APickupSpawner* Spawner);

	UFUNCTION()
		ASpawner* DetermineBestSpawner();

	UFUNCTION()
		APickupSpawner* DetermineBestPickupSpawner() const;

	UFUNCTION(BlueprintCallable)
		ASpawner* GetClosestSpawnerToLocation(FVector Location) const;

private:
	UFUNCTION()
		void UpdateEnemySpawns(float DeltaSeconds);

	UFUNCTION()
		void UpdatePickupSpawns(float DeltaSeconds);

	UFUNCTION()
		bool TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor);

	UFUNCTION()
		void ResetSpawnTime();

};
