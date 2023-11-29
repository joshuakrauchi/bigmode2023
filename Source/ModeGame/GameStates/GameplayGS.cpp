// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGS.h"
#include "Spawning/SpawnManagerComponent.h"

AGameplayGS::AGameplayGS()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnManagerComponent = CreateDefaultSubobject<USpawnManagerComponent>(FName("SpawnManagerComponent"));
}

void AGameplayGS::BeginPlay()
{
	Super::BeginPlay();
}

USpawnManagerComponent* AGameplayGS::GetSpawnManager()
{
	return SpawnManagerComponent;
}
