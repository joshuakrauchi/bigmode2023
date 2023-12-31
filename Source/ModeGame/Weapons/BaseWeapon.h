// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Fireable.h"
#include "Utils/EPlayableColours.h"
#include "BaseWeapon.generated.h"

class UAnimSequence;
class UAimOffsetBlendSpace;
class USkeletalMeshComponent;
class IDamageable;
class UNiagaraSystem;

UCLASS()
class MODEGAME_API ABaseWeapon : public AActor, public IFireable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxRange = 0.0f;

	UPROPERTY(EditAnywhere)
		float BaseDamage = 0.0f;

	UPROPERTY(EditAnywhere)
		float MinimumDamage = 0.0f;

	UPROPERTY(EditAnywhere)
		float DamageFalloffRange = 0.0f;

	UPROPERTY(EditAnywhere)
		float DamageFalloffEndRange = 0.0f;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UMaterialInterface> DecalMaterial = nullptr;

	UPROPERTY(EditAnywhere)
		float DecalSize = 0.0f;

	UPROPERTY(EditAnywhere)
		float DecalLifetime = 0.0f;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UNiagaraSystem> ImpactSparksSystem = nullptr;
	
	const FTransform* EquippedTransform = nullptr;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UAnimSequence> ArmedAnimSequence = nullptr;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<UAimOffsetBlendSpace> ArmedAimOffset = nullptr;

	UPROPERTY(EditAnywhere)
		FTransform AttachmentTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<USceneComponent> EmptyRoot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<USkeletalMeshComponent> FirstPersonMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<USkeletalMeshComponent> ThirdPersonMesh = nullptr;

	UPROPERTY()
		bool bIsHoldingFire = false;

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

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

	virtual bool TryEquipToParentTransform(const FTransform& Transform, USkeletalMeshComponent* FPMeshAttachment, USkeletalMeshComponent* TPMeshAttachment, FName SocketName) override;
	// end IFireable

	UFUNCTION(BlueprintCallable)
		void EndFireOnHealthDepleted();

	UFUNCTION(BlueprintCallable)
		virtual float GetFalloffAdjustedDamage(float Distance) const;

	virtual UAnimSequence* GetArmedAnimSequence_Implementation() const override;

	virtual UAimOffsetBlendSpace* GetArmedAimOffset_Implementation() const override;

	UFUNCTION(BlueprintCallable)
		const FTransform& GetEquippedTransform() { return *EquippedTransform; }

	UFUNCTION(BlueprintCallable)
		USkeletalMeshComponent* GetVisibleMesh() const;

	UFUNCTION(BlueprintCallable)
		bool TryDamageDamageable(TScriptInterface<IDamageable> Damageable, FVector DamageStartLocation, float Distance);

	UFUNCTION(BlueprintCallable)
		bool IsHoldingFire() const;

protected:
	UFUNCTION()
		void SpawnDecal(FVector Location, FVector Normal, USceneComponent* AttachComponent);

	UFUNCTION()
		void SpawnImpactSparks(FVector Location);

};
