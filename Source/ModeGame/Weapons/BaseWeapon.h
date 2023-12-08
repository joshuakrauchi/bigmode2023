// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Fireable.h"
<<<<<<< HEAD
#include "Utils/EPlayableColours.h"
=======
>>>>>>> eaa58272dadaeb6eaf2acd1f485773f1c6bd076d
#include "BaseWeapon.generated.h"

class UAnimSequence;
class UAimOffsetBlendSpace;
class USkeletalMeshComponent;

UCLASS()
class MODEGAME_API ABaseWeapon : public AActor, public IFireable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		float MaxRange = 0.0f;
	UPROPERTY(EditAnywhere)
		float BaseDamage = 0.0f;
	UPROPERTY(EditAnywhere)
		float DamageFalloffRange = 0.0f;
	UPROPERTY(EditAnywhere)
		EPlayableColours Colour = EPlayableColours::None;
	
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

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
<<<<<<< HEAD
	ABaseWeapon(float MaxRange,
				EPlayableColours Colour) 
		: MaxRange(MaxRange),
		  Colour(Colour),
		  EquippedTransform(nullptr)
	{};

protected:
=======
>>>>>>> eaa58272dadaeb6eaf2acd1f485773f1c6bd076d
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// begin IFireable
	virtual bool TryBeginFire() override;

	virtual bool TryEndFire() override;

<<<<<<< HEAD
	virtual bool TryEquipToParentTransform(const FTransform& Transform) override;
	// end IFireable

public:
	virtual float GetFalloffAdjustedDamage(float Distance);
=======
	virtual bool TryEquipToParentTransform(const FTransform& Transform, USkeletalMeshComponent* FPMeshAttachment, USkeletalMeshComponent* TPMeshAttachment, FName SocketName) override;

	virtual UAnimSequence* GetArmedAnimSequence_Implementation() const override;

	virtual UAimOffsetBlendSpace* GetArmedAimOffset_Implementation() const override;
>>>>>>> eaa58272dadaeb6eaf2acd1f485773f1c6bd076d

public:
	UFUNCTION(BlueprintCallable)
		const FTransform& GetEquippedTransform() { return *EquippedTransform; }

	UFUNCTION(BlueprintCallable)
		USkeletalMeshComponent* GetVisibleMesh() const;

};
