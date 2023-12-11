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
	bIsHoldingFire = true;

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
		FVector RandomSpread{ FMath::RandRange(-MaxSpreadRadians, MaxSpreadRadians), FMath::RandRange(-MaxSpreadRadians, MaxSpreadRadians), FMath::RandRange(-MaxSpreadRadians, MaxSpreadRadians) };
		FVector AdjustedForward = (ParentForward + RandomSpread) * MaxRange;

		World->LineTraceSingleByChannel(Trace, TraceStart, ParentLocation + AdjustedForward, ECollisionChannel::ECC_Visibility, CollisionParams);
		if (!Trace.IsValidBlockingHit())
		{
			SpawnBulletTrail(AdjustedForward);
			continue;
		}

		SpawnBulletTrail(Trace.ImpactPoint);

		TScriptInterface<IDamageable> Damageable = nullptr;
		Damageable.SetInterface(Cast<IDamageable>(Trace.GetActor()));
		if (Damageable != nullptr)
		{
			TryDamageDamageable(Damageable, Trace.Distance);
		}
		else
		{
			SpawnDecal(Trace.ImpactPoint, Trace.ImpactNormal, Trace.GetComponent());

			if (IsValid(ImpactSparksSystem))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, ImpactSparksSystem, Trace.ImpactPoint);
			}
		}

		// Visual debug component
		//DrawDebugLine(World, ParentLocation, ParentLocation + AdjustedForward, { 255, 0, 0 }, false, 3.0f, 0U, 0.2f);
	}

	PlayFiringMontage();
	CurrentShotCooldownTime = BaseShotCooldownTime;

	// Do trace stuff
	ReceiveTestFire();

	return Super::TryBeginFire();
}

bool AGreenWeapon::TryEndFire()
{
	bIsHoldingFire = false;

	return Super::TryEndFire();
}

bool AGreenWeapon::CanFire() const
{
	return (CurrentShotCooldownTime <= 0.0f);
}

void AGreenWeapon::SpawnDecal(FVector Location, FVector Normal, USceneComponent* AttachComponent)
{
	if (!IsValid(DecalMaterial)) { return; }
	if (!IsValid(AttachComponent)) { return; }

	TObjectPtr<UDecalComponent> Decal = UGameplayStatics::SpawnDecalAttached(
		DecalMaterial,
		FVector(DecalSize),
		AttachComponent,
		NAME_None,
		Location,
		Normal.ToOrientationRotator(),
		EAttachLocation::KeepWorldPosition,
		300.0f
	);
	
	if (!IsValid(Decal)) { return; }

	Decal->SetFadeScreenSize(0.0f);
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
