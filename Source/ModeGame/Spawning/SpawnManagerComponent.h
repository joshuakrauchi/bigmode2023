// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnManagerComponent.generated.h"

class ASpawner;

UCLASS()
class MODEGAME_API USpawnManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TArray<TObjectPtr<UClass>> PossibleSpawns;

	UPROPERTY(EditAnywhere)
		float SpawnMultiplierBaseToPowOfNumCharacters = 1.0f;

	UPROPERTY(EditAnywhere)
		float BaseSpawnTime = 1.0f;

	UPROPERTY()
		float CurrentSpawnTime = 0.0f;

	UPROPERTY()
		TArray<TObjectPtr<ASpawner>> Spawners;

	UPROPERTY(EditAnywhere)
		int MinCharactersSpawned = 0;

	UPROPERTY(EditAnywhere)
		int MaxCharactersSpawned = 0;

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
		ASpawner* DetermineBestSpawner();

	UFUNCTION(BlueprintCallable)
		ASpawner* GetClosestSpawnerToLocation(FVector Location) const;

private:
	UFUNCTION()
		bool TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor);

	UFUNCTION()
		void ResetSpawnTime();

};
