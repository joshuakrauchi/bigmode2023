// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenWeapon.h"
#include "Math/UnrealMathUtility.h"

AGreenWeapon::AGreenWeapon()
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

		// Finds hits
		for (int i = 0; i < MaxPellets; ++i)
		{
			FVector RandomSpread { FMath::RandRange(-MaxSpreadRadians, MaxSpreadRadians), FMath::RandRange(-MaxSpreadRadians, MaxSpreadRadians), FMath::RandRange(-MaxSpreadRadians, MaxSpreadRadians) };
			FVector AdjustedForward = (ParentForward + RandomSpread) * MaxRange;

			World->LineTraceSingleByChannel(Trace, ParentLocation, ParentLocation + AdjustedForward, ECollisionChannel::ECC_Visibility);

			// Visual debug component
			DrawDebugLine(World, ParentLocation, ParentLocation + AdjustedForward, { 255, 0, 0 }, false, 3.0f, 0U, 0.2f);
		}
	}

	// Do trace stuff
	ReceiveTestFire();

	return Super::TryBeginFire() && true;
}


bool AGreenWeapon::TryEndFire()
{
	return Super::TryEndFire() && true;
}

