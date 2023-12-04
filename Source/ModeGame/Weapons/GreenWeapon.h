// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "GreenWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MODEGAME_API AGreenWeapon : public ABaseWeapon
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		int MaxPellets = 8;
	UPROPERTY(EditAnywhere)
		float MaxSpreadRadians = 0.1f;
	
public:
	AGreenWeapon();

protected:
	virtual void BeginPlay() override;

public:
	// Begin IFireable Interface
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool TryBeginFire() override;

	virtual bool TryEndFire() override;
	// End IFireable Interface

	UFUNCTION(BlueprintImplementableEvent)
		bool ReceiveTestFire();
};
