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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BaseHealth = 0;

	UPROPERTY(BlueprintReadOnly)
		float CurrentHealth = 0;

	UPROPERTY(BlueprintReadOnly)
		int64 CurrentScore = 0.0f;

private:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USpawnManagerComponent> SpawnManagerComponent = nullptr;

public:
	AGameplayGS();

	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable)
		USpawnManagerComponent* GetSpawnManager() const;

	UFUNCTION(BlueprintCallable)
		void IncreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable)
		void DecreaseHealth(float Amount);

	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthDepleted();

	UFUNCTION(BlueprintCallable)
		int GetScore();

	UFUNCTION(BlueprintCallable)
		void AddScore(int ScoreAmount);
};
