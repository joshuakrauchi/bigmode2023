// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawner.generated.h"

class USceneComponent;

UCLASS()
class MODEGAME_API APickupSpawner : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
		float TimeAtLastSpawn = 0.0f;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USceneComponent> EmptyRoot = nullptr;
	
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USceneComponent> SpawnPoint = nullptr;

	UPROPERTY()
		TObjectPtr<AActor> SpawnedPickup = nullptr;

public:
	// Sets default values for this actor's properties
	APickupSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		bool TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor);

	UFUNCTION()
		float GetSecondsSinceLastSpawn() const;

	UFUNCTION()
		bool HasSpawnedPickup() const;

private:
	UFUNCTION()
		bool TryRegisterWithSpawnManager();

};
