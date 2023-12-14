// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "GreenWeapon.generated.h"

class UMaterialInterface;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class MODEGAME_API AGreenWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BaseShotCooldownTime = 0.0f;

	UPROPERTY(BlueprintReadOnly)
		float CurrentShotCooldownTime = 0.0f;

private:
	UPROPERTY(EditAnywhere)
		int MaxPellets = 8;

	UPROPERTY(EditAnywhere)
		float MaxSpreadDegrees = 0.1f;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UNiagaraSystem> BulletTrailSystem = nullptr;

public:
	AGreenWeapon();

protected:
	virtual void BeginPlay() override;

public:
	// Begin IFireable Interface
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual bool TryBeginFire() override;

	virtual bool TryEndFire() override;
	// End IFireable Interface

	UFUNCTION(BlueprintImplementableEvent)
		void PlayFiringMontage();

	UFUNCTION(BlueprintImplementableEvent)
		bool ReceiveTestFire();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayFiringSFX();

	UFUNCTION(BlueprintCallable)
		bool CanFire() const;

private:
	UFUNCTION()
		void SpawnBulletTrail(FVector BeamEnd);

};
