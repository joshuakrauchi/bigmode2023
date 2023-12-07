// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Fireable.h"
#include "BaseWeapon.generated.h"

class UAnimSequence;
class UAimOffsetBlendSpace;

UCLASS()
class MODEGAME_API ABaseWeapon : public AActor, public IFireable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		float MaxRange = 0.0f;
	
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
		TObjectPtr<UStaticMeshComponent> FPMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UStaticMeshComponent> TPMesh = nullptr;

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool TryBeginFire() override;

	virtual bool TryEndFire() override;

	virtual bool TryEquipToParentTransform(const FTransform& Transform, USkeletalMeshComponent* FPMeshAttachment, USkeletalMeshComponent* TPMeshAttachment, FName SocketName) override;

	virtual UAnimSequence* GetArmedAnimSequence_Implementation() const override;

	virtual UAimOffsetBlendSpace* GetArmedAimOffset_Implementation() const override;

public:
	UFUNCTION(BlueprintCallable)
		const FTransform& GetEquippedTransform() { return *EquippedTransform; }

};
