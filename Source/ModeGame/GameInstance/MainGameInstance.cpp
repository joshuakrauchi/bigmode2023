// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MainGameInstance.h"

bool UMainGameInstance::IsHardMode() const
{
	return bIsHardMode;
}

void UMainGameInstance::SetIsHardMode(bool bHardMode)
{
	bIsHardMode = bHardMode;
}
