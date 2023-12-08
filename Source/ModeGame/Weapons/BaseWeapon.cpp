// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABaseWeapon::TryBeginFire()
{
	UE_LOG(LogTemp, Warning, TEXT("BEGIN FIRE"));

	return true;
}

bool ABaseWeapon::TryEndFire()
{
	UE_LOG(LogTemp, Warning, TEXT("END FIRE"));

	return true;
}

float ABaseWeapon::GetFalloffAdjustedDamage(float Distance)
{
	return BaseDamage * (1 / Distance);
}

bool ABaseWeapon::TryEquipToParentTransform(const FTransform& Transform)
{
	return (EquippedTransform = &Transform) != nullptr;
}

