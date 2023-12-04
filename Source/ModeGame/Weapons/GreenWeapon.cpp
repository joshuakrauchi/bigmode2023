// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenWeapon.h"

AGreenWeapon::AGreenWeapon() : ABaseWeapon(10000.0f)
{
}

void AGreenWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AGreenWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGreenWeapon::TryBeginFire()
{
	FVector ParentLocation = EquippedTransform->GetLocation();
	FVector ParentForward = EquippedTransform->GetRotation().GetForwardVector();

	UWorld* World = GetWorld();

	if (IsValid(World)) 
	{
		FHitResult Trace;
		World->LineTraceSingleByChannel(Trace, ParentLocation, ParentLocation + (ParentForward * MaxRange), ECollisionChannel::ECC_Visibility);
		DrawDebugLine(World, ParentLocation, ParentLocation + (ParentForward * MaxRange), { 255, 0, 0 }, false, 3.0f, 0U, 1.0f);
	}

	// Do trace stuff
	// ReceiveTestFire();

	return Super::TryBeginFire() && false;
}


bool AGreenWeapon::TryEndFire()
{
	return Super::TryEndFire() && false;
}

