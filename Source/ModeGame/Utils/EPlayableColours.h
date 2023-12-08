// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EPlayableColours.generated.h"

/**
 * This class does not need to be modified
 */
UCLASS()
class MODEGAME_API UPlayableColours : public UUserDefinedEnum
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EPlayableColours : uint8
{
	None,

	Grey,
	Red,
	Green,
	Blue,

	Count,
};
