// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "RedWeapon.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class MODEGAME_API ARedWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float ShotRadius = 0.0f;

	UPROPERTY(EditAnywhere)
		float BaseTimeBetweenDamages = 0.0f;

	UPROPERTY()
		float CurrentTimeBetweenDamages = 0.0f;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UNiagaraComponent> FPNiagaraComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UNiagaraComponent> TPNiagaraComponent = nullptr;

private:
	UPROPERTY()
		float StopShootingParticleDelay = 0.0f;

public:
	ARedWeapon();

	virtual void Tick(float DeltaSeconds) override;

	virtual bool TryBeginFire() override;

	virtual bool TryEndFire() override;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayFiringSFX();

	UFUNCTION(BlueprintImplementableEvent)
		void StopFiringSFX();

private:
	UFUNCTION()
		void SetBeamEnd(FVector BeamEnd);

	UFUNCTION()
		void UpdateShooting(float DeltaSeconds);

	UFUNCTION()
		UNiagaraComponent* GetRelevantParticleComponent() const;

};
