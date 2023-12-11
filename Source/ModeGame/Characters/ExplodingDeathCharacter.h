// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "ExplodingDeathCharacter.generated.h"

class USphereComponent;
class URadialVector;
class URadialFalloff;
class UGeometryCollectionComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UMaterialInterface;

/**
 * 
 */
UCLASS()
class MODEGAME_API AExplodingDeathCharacter : public AFieldSystemActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<USphereComponent> Collision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UNiagaraComponent> ExplosionParticleComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<URadialFalloff> RadialFalloff = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<URadialVector> RadialVector = nullptr;

	UPROPERTY(EditAnywhere)
		float ExplodeVelocity = 0.0f;

public:
	AExplodingDeathCharacter();

protected:
	virtual void BeginPlay() override;

public:
	void ApplyMaterial(TObjectPtr<UMaterialInterface> Material);

};
