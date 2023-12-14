// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RedWeapon.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ARedWeapon::ARedWeapon()
{
	FPNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("FPNiagaraComponent"));
	FPNiagaraComponent->SetupAttachment(FirstPersonMesh, FName("Muzzle"));

	TPNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("TPNiagaraComponent"));
	TPNiagaraComponent->SetupAttachment(ThirdPersonMesh, FName("Muzzle"));
}

void ARedWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateShooting(DeltaSeconds);
}

bool ARedWeapon::TryBeginFire()
{
	Super::TryBeginFire();

	PlayFiringSFX();
	TObjectPtr<UNiagaraComponent> ParticleComponent = GetRelevantParticleComponent();
	if (IsValid(ParticleComponent))
	{
		ParticleComponent->Activate();
	}

	return true;
}

bool ARedWeapon::TryEndFire()
{
	Super::TryEndFire();

	if (IsValid(FPNiagaraComponent))
	{
		FPNiagaraComponent->Deactivate();
	}
	if (IsValid(TPNiagaraComponent))
	{
		TPNiagaraComponent->Deactivate();
	}

	StopFiringSFX();

	return true;
}

void ARedWeapon::SetBeamEnd(FVector BeamEnd)
{
	TObjectPtr<UNiagaraComponent> ParticleComponent = GetRelevantParticleComponent();
	if (!IsValid(ParticleComponent)) { return; }

	ParticleComponent->SetVectorParameter(FName("Beam End"), BeamEnd);
}

void ARedWeapon::UpdateShooting(float DeltaSeconds)
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	TObjectPtr<ABaseCharacter> Character = GetOwner<ABaseCharacter>();
	if (!IsValid(Character)) { return; }

	CurrentTimeBetweenDamages -= DeltaSeconds;

	FVector TraceStart = Character->GetProjectileStartLocation();
	TraceStart += (Character->GetControlRotation().Vector() * 80.0f);
	FVector TraceEnd = Character->GetProjectileEndLocation(MaxRange, 0.0f);

	if (!IsHoldingFire())
	{
		StopShootingParticleDelay -= DeltaSeconds;

		if (StopShootingParticleDelay > 0.0f)
		{
			SetBeamEnd(TraceEnd);
		}

		return;
	}

	StopShootingParticleDelay = 0.75f;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	FHitResult OutHit;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStart, TraceEnd, ShotRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

	if (!OutHit.IsValidBlockingHit())
	{
		SetBeamEnd(TraceEnd);
		return;
	}

	SetBeamEnd(OutHit.ImpactPoint);

	TScriptInterface<IDamageable> Damageable = OutHit.GetActor();
	if (Damageable != nullptr)
	{
		if (CurrentTimeBetweenDamages <= 0.0f)
		{
			CurrentTimeBetweenDamages = BaseTimeBetweenDamages;
			TryDamageDamageable(Damageable, TraceStart, OutHit.Distance);
		}
	}
	else
	{
		SpawnDecal(OutHit.ImpactPoint, OutHit.ImpactNormal, OutHit.GetComponent());

		SpawnImpactSparks(OutHit.ImpactPoint);
	}
}

UNiagaraComponent* ARedWeapon::GetRelevantParticleComponent() const
{
	TObjectPtr<APawn> Pawn = GetOwner<APawn>();
	if (!IsValid(Pawn)) { return nullptr; }

	if (Pawn->IsPlayerControlled())
	{
		return FPNiagaraComponent;
	}
	else
	{
		return TPNiagaraComponent;
	}
}
