// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpawnerComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MODEGAME_API USpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
		float TimeAtLastSpawn = 0.0f;

public:
	// Sets default values for this component's properties
	USpawnerComponent();

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
