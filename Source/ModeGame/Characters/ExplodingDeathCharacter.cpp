// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ExplodingDeathCharacter.h"
#include "Field/FieldSystemObjects.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemComponent.h"
#include "NiagaraComponent.h"

AExplodingDeathCharacter::AExplodingDeathCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(FName("GeometryCollectionComponent"));
	GeometryCollectionComponent->SetupAttachment(GetRootComponent());

	Collision = CreateDefaultSubobject<USphereComponent>(FName("Collision"));
	Collision->SetupAttachment(GeometryCollectionComponent);

	ExplosionParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("ExplosionParticleComponent"));
	ExplosionParticleComponent->SetupAttachment(GeometryCollectionComponent);

	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(FName("RadialFalloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(FName("RadialVector"));
}

void AExplodingDeathCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(FieldSystemComponent)) { return; }
	if (!IsValid(Collision)) { return; }

	TObjectPtr<URadialFalloff> ExternalStrain = RadialFalloff->SetRadialFalloff(10000.0f, 0.0f, 1.0f, 0.0f, Collision->GetScaledSphereRadius(), Collision->GetComponentLocation(), EFieldFalloffType::Field_FallOff_None);
	TObjectPtr<URadialVector> LinearVelocity = RadialVector->SetRadialVector(ExplodeVelocity, Collision->GetComponentLocation());
	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, ExternalStrain);
	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearVelocity, nullptr, LinearVelocity);
}

void AExplodingDeathCharacter::ApplyMaterial(TObjectPtr<UMaterialInterface> Material, FLinearColor ParticleColor)
{
	if (!IsValid(GeometryCollectionComponent)) { return; }
	if (!IsValid(ExplosionParticleComponent)) { return; }

	GeometryCollectionComponent->SetMaterial(0, Material);

	ExplosionParticleComponent->SetVectorParameter(FName("Scale RGB"), FVector(ParticleColor.R, ParticleColor.G, ParticleColor.B));
}
