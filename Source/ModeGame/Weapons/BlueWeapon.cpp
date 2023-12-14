// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BlueWeapon.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

void ABlueWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateShotScatter(DeltaSeconds);
	UpdateShooting(DeltaSeconds);
}

bool ABlueWeapon::TryBeginFire()
{
	Super::TryBeginFire();

	PlayFiringSFX();

	return true;
}

bool ABlueWeapon::TryEndFire()
{
	Super::TryEndFire();

	StopFiringSFX();

	return true;
}

void ABlueWeapon::UpdateShotScatter(float DeltaSeconds)
{
	CurrentShotScatterRange = FMath::Max(0.0f, CurrentShotScatterRange - (ScatterCorrectionSpeed * DeltaSeconds));
}

void ABlueWeapon::SpawnBulletTrail(FVector BeamEnd)
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

void ABlueWeapon::UpdateShooting(float DeltaSeconds)
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	TObjectPtr<ABaseCharacter> Character = GetOwner<ABaseCharacter>();
	if (!IsValid(Character)) { return; }

	CurrentShootTime -= DeltaSeconds;

	if (!IsHoldingFire() || (CurrentShootTime > 0.0f)) { return; }

	CurrentShootTime = BaseShootTime;

	PlayFiringMontage();

	FVector TraceStart = Character->GetProjectileStartLocation();
	FVector TraceEnd = Character->GetProjectileEndLocation(MaxRange, CurrentShotScatterRange);

	CurrentShotScatterRange = FMath::Min(MaxScatter, ShotScatterIncreasePerShot + CurrentShotScatterRange);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	FHitResult OutHit;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStart, TraceEnd, ShotRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

	if (!OutHit.IsValidBlockingHit())
	{
		SpawnBulletTrail(TraceEnd);
		return;
	}

	SpawnBulletTrail(OutHit.ImpactPoint);

	TScriptInterface<IDamageable> Damageable = OutHit.GetActor();
	if (Damageable != nullptr)
	{
		TryDamageDamageable(Damageable, TraceStart, OutHit.Distance);
	}
	else
	{
		SpawnDecal(OutHit.ImpactPoint, OutHit.ImpactNormal, OutHit.GetComponent());

		if (IsValid(ImpactSparksSystem))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, ImpactSparksSystem, OutHit.ImpactPoint);
		}
	}
}
