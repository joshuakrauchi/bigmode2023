// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PointsPickup.h"
#include "GameStates/GameplayGS.h"

bool APointsPickup::TryApplyPlayerPickupEffect()
{
	if (!Super::TryApplyPlayerPickupEffect()) { return false; }

	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return false; }

	TObjectPtr<AGameplayGS> GameplayGS = World->GetGameState<AGameplayGS>();
	if (!IsValid(GameplayGS)) { return false; }

	GameplayGS->AddScore(PointsGiven);
	GameplayGS->IncrementComboCount();

	return true;
}
