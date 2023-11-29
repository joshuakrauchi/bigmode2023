// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAIC.h"
#include "BehaviorTree/BehaviorTree.h"

AGameplayAIC::AGameplayAIC()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameplayAIC::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
