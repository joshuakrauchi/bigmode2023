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
		float MaxSpreadRadians = 0.1f;

	UPROPERTY(EditAnywhere)
		float DecalSize = 0.0f;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<UMaterialInterface> DecalMaterial = nullptr;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UNiagaraSystem> ImpactSparksSystem = nullptr;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UNiagaraSystem> BulletTrailSystem = nullptr;

	UPROPERTY()
		bool bIsHoldingFire = false;

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

	UFUNCTION(BlueprintCallable)
		bool CanFire() const;

private:
	UFUNCTION(BlueprintCallable)
		void SpawnDecal(FVector Location, FVector Normal, USceneComponent* AttachComponent);

	UFUNCTION(BlueprintCallable)
		void SpawnBulletTrail(FVector BeamEnd);

};
