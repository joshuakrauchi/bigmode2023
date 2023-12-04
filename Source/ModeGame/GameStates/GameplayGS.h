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
	
private:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USpawnManagerComponent> SpawnManagerComponent = nullptr;

public:
	AGameplayGS();

	virtual void BeginPlay();

	UFUNCTION()
		USpawnManagerComponent* GetSpawnManager();

};
