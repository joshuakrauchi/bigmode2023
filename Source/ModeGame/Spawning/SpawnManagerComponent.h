// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnManagerComponent.generated.h"

class ASpawner;

USTRUCT()
struct FSpawnInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TObjectPtr<UClass> ActorClass = nullptr;

	UPROPERTY(EditAnywhere)
		float InitialTimeBeforeSpawn = 0.0f;

	UPROPERTY(EditAnywhere)
		float BaseTimeBetweenSpawns = 0.0f;
};

UCLASS()
class MODEGAME_API USpawnManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TArray<FSpawnInfo> SpawnInfos;

	UPROPERTY(EditAnywhere)
		float BaseSpawnTimeMultiplier = 1.0f;

	UPROPERTY()
		float CurrentSpawnTimeMultiplier = 1.0f;

	UPROPERTY()
		TMap<TObjectPtr<UClass>, float> TimeBeforeSpawnByClass;

	UPROPERTY()
		TArray<TObjectPtr<ASpawner>> Spawners;

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

private:
	bool TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor);

};
