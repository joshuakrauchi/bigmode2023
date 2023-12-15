// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/DirectionalLight.h"
#include "GameplayGS.generated.h"

class USpawnManagerComponent;
class ABaseCharacter;
class UBlackboardComponent;

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

	UPROPERTY(EditAnywhere)
		float ScoreMultiplierIncreasePerComboKill = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BaseTimeUntilComboEnd = 0.0f;

	UPROPERTY(BlueprintReadOnly)
		float CurrentTimeUntilComboEnd = 0.0f;

	UPROPERTY(BlueprintReadOnly)
		int CurrentComboCount = 0;

	UPROPERTY(EditAnywhere)
		int DuskScoreAmount = 0;

	UPROPERTY(EditAnywhere)
		int NightScoreAmount = 0;

	UPROPERTY(EditAnywhere)
		float StartingSunRotation = 0.f;

	UPROPERTY(EditAnywhere)
		float NightSunRotation = 0.f;

	UPROPERTY(EditAnywhere)
		float DuskSunRotation = 0.f;

private:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USpawnManagerComponent> SpawnManagerComponent = nullptr;

	UPROPERTY()
		TSet<TObjectPtr<ABaseCharacter>> AllCharacters;

	UPROPERTY()
		TObjectPtr<ADirectionalLight> LevelDirectionalLight = nullptr;

public:
	AGameplayGS();

	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

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

	UFUNCTION(BlueprintImplementableEvent)
		void OnComboIncreased();

	UFUNCTION()
		void RegisterCharacter(ABaseCharacter* Character);

	UFUNCTION()
		void DeregisterCharacter(ABaseCharacter* Character);

	UFUNCTION()
		int GetNumCharacters() const;

	UFUNCTION(BlueprintCallable)
		void IncrementComboCount();

	UFUNCTION()
		void ClearComboCount();

private:
	UFUNCTION()
		void UpdateComboEndTime(float DeltaSeconds);

};
