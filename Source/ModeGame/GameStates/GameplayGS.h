// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameplayGS.generated.h"

class USpawnManagerComponent;

/**
 * 
 */
UCLASS()
class MODEGAME_API AGameplayGS : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float BaseHealth = 0;

private:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USpawnManagerComponent> SpawnManagerComponent = nullptr;

	UPROPERTY()
		float CurrentHealth = 0;

public:
	AGameplayGS();

	virtual void BeginPlay();

	UFUNCTION()
		USpawnManagerComponent* GetSpawnManager() const;

	UFUNCTION(BlueprintCallable)
		void IncreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable)
		void DecreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable)
		void OnHealthDepleted();

};
