// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenWeapon.h"
#include "Math/UnrealMathUtility.h"
#include "Characters/BaseCharacter.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

AGreenWeapon::AGreenWeapon()
{
	MaxRange = 10000.0f;
	BaseDamage = 5.0f;
}

void AGreenWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AGreenWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentShotCooldownTime = FMath::Max(0.0f, (CurrentShotCooldownTime - DeltaSeconds));

	if (bIsHoldingFire)
	{
		TryBeginFire();
	}
}

bool AGreenWeapon::TryBeginFire()
{
	if (!CanFire()) { return false; }

	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return false; }
	
	TObjectPtr<ABaseCharacter> BaseCharacter = GetOwner<ABaseCharacter>();
	if (!IsValid(BaseCharacter)) { return false; }

	FVector TraceStart = BaseCharacter->GetProjectileStartLocation();

	FVector ParentLocation = EquippedTransform->GetLocation();
	FVector ParentForward = EquippedTransform->GetRotation().GetForwardVector();

	FHitResult Trace{ };
	FCollisionQueryParams CollisionParams{};
	CollisionParams.AddIgnoredActor(GetOwner());

	// Finds hits
	for (int i = 0; i < MaxPellets; ++i)
	{
		// Halve spread for half of the pellets.
		float Spread = MaxSpreadDegrees;
		if (i < MaxPellets / 2.0f)
		{
			Spread /= 2.0f;

			// Shoot at least one straight.
			if (i == 0)
			{
				Spread = 0.0f;
			}
		}

		FVector AdjustedForward = BaseCharacter->GetProjectileEndLocation(MaxRange, Spread);

		World->LineTraceSingleByChannel(Trace, TraceStart, AdjustedForward, ECollisionChannel::ECC_Visibility, CollisionParams);
		if (!Trace.IsValidBlockingHit())
		{
			SpawnBulletTrail(ParentLocation + AdjustedForward);
			continue;
		}

		SpawnBulletTrail(Trace.ImpactPoint);

		TScriptInterface<IDamageable> Damageable = Trace.GetActor();
		if (Damageable != nullptr)
		{
			TryDamageDamageable(Damageable, TraceStart, Trace.Distance);
		}
		else
		{
			SpawnDecal(Trace.ImpactPoint, Trace.ImpactNormal, Trace.GetComponent());

			SpawnImpactSparks(Trace.ImpactPoint);
		}
	}

	PlayFiringMontage();
	CurrentShotCooldownTime = BaseShotCooldownTime;

	// Do trace stuff
	ReceiveTestFire();

	return Super::TryBeginFire();
}

bool AGreenWeapon::TryEndFire()
{
	return Super::TryEndFire();
}

bool AGreenWeapon::CanFire() const
{
	return (CurrentShotCooldownTime <= 0.0f);
}

void AGreenWeapon::SpawnBulletTrail(FVector BeamEnd)
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	TObjectPtr<USkeletalMeshComponent> Mesh = GetVisibleMesh();
	if (!IsValid(Mesh)) { return; }

	FVector MuzzleLocation = Mesh->GetSocketLocation(FName("Muzzle"));

	TObjectPtr<UNiagaraComponent> BulletTrail = UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, BulletTrailSystem, MuzzleLocation);
	if (!IsValid(BulletTrail)) { return; }

	BulletTrail->SetVectorParameter(FName("Beam End"), BeamEnd);
}
