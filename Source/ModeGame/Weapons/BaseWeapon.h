// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Fireable.h"
#include "Utils/EPlayableColours.h"
#include "BaseWeapon.generated.h"

UCLASS()
class MODEGAME_API ABaseWeapon : public AActor, public IFireable
{
	GENERATED_BODY()

protected:
	const FTransform* EquippedTransform = nullptr;

	UPROPERTY(EditAnywhere)
		float MaxRange = 0.0f;
	UPROPERTY(EditAnywhere)
		float BaseDamage = 0.0f;
	UPROPERTY(EditAnywhere)
		float DamageFalloffRange = 0.0f;
	UPROPERTY(EditAnywhere)
		EPlayableColours Colour = EPlayableColours::None;
	
public:
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	ABaseWeapon(float MaxRange,
				EPlayableColours Colour) 
		: MaxRange(MaxRange),
		  Colour(Colour),
		  EquippedTransform(nullptr)
	{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// begin IFireable
	virtual bool TryBeginFire() override;

	virtual bool TryEndFire() override;

	virtual bool TryEquipToParentTransform(const FTransform& Transform) override;
	// end IFireable

public:
	virtual float GetFalloffAdjustedDamage(float Distance);

public:
	UFUNCTION(BlueprintCallable)
		const FTransform& GetEquippedTransform() { return *EquippedTransform; }
};
