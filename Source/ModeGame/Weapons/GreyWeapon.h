// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "GreyWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MODEGAME_API AGreyWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	virtual bool TryBeginFire() override;

	virtual bool TryEndFire() override;

	UFUNCTION(BlueprintImplementableEvent)
		bool ReceiveTestFire();

	UFUNCTION(BlueprintImplementableEvent)
		bool ReceiveEndFire();

};
