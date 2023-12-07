// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Fireable.generated.h"

class UAimOffsetBlendSpace;
class UAnimSequence;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UFireable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODEGAME_API IFireable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
		virtual bool TryBeginFire() PURE_VIRTUAL(IFireable::TryBeginFire, return false;);

	UFUNCTION()
		virtual bool TryEndFire() PURE_VIRTUAL(IFireable::TryEndFire, return false;);

	UFUNCTION()
		virtual bool TryEquipToParentTransform(const FTransform& Transform, USkeletalMeshComponent* FPMeshAttachment, USkeletalMeshComponent* TPMeshAttachment, FName SocketName) PURE_VIRTUAL(IFireable::TryEquipToParentTransform, return false;);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		UAnimSequence* GetArmedAnimSequence() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		UAimOffsetBlendSpace* GetArmedAimOffset() const;

};
