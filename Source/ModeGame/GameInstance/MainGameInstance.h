// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MODEGAME_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite)
		bool bIsHardMode = false;

public:
	UFUNCTION(BlueprintCallable)
		bool IsHardMode() const;

	UFUNCTION(BlueprintCallable)
		void SetIsHardMode(bool bHardMode);

};
