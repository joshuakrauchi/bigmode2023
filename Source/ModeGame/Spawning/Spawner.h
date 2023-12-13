// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;


UCLASS()
class MODEGAME_API ASpawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
		float TimeAtLastSpawn = 0.0f;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UCapsuleComponent> SpawnCapsule = nullptr;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UNiagaraComponent> SpawnParticleComponent = nullptr;

public:
	// Sets default values for this component's properties
	ASpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		bool TrySpawnActor(UClass* ActorClass, AActor*& OutSpawnedActor);

	UFUNCTION()
		float GetSecondsSinceLastSpawn() const;
		
private:
	UFUNCTION()
		bool TryRegisterWithSpawnManager();

};
