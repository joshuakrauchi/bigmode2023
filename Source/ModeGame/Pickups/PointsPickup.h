// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BasePickup.h"
#include "PointsPickup.generated.h"

/**
 * 
 */
UCLASS()
class MODEGAME_API APointsPickup : public ABasePickup
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		int PointsGiven = 0;

protected:
	virtual bool TryApplyPlayerPickupEffect() override;
	
};
