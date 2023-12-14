// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "BlueWeapon.generated.h"

class UNiagaraSystem;
class UMaterialInterface;

/**
 * 
 */
UCLASS()
class MODEGAME_API ABlueWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
		float CurrentShotScatterRange = 0.0f;

private:
	UPROPERTY(EditAnywhere)
		float ScatterCorrectionSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
		float MaxScatter = 0.0f;

	UPROPERTY(EditAnywhere)
		float ShotScatterIncreasePerShot = 0.0f;

	UPROPERTY(EditAnywhere)
		float ShotRadius = 0.0f;

	UPROPERTY(EditAnywhere)
		float BaseShootTime = 0.0f;

	UPROPERTY()
		float CurrentShootTime = 0.0f;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UNiagaraSystem> BulletTrailSystem = nullptr;

public:
	virtual void Tick(float DeltaSeconds) override;
	
	virtual bool TryBeginFire() override;

	virtual bool TryEndFire() override;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayFiringMontage();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayFireSFX();

private:
	UFUNCTION()
		void UpdateShotScatter(float DeltaSeconds);

	UFUNCTION()
		void SpawnBulletTrail(FVector BeamEnd);

	UFUNCTION()
		void UpdateShooting(float DeltaSeconds);

};
