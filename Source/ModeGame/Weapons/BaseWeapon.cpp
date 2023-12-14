// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/BaseCharacter.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EmptyRoot = CreateDefaultSubobject<USceneComponent>(FName("EmptyRoot"));
	SetRootComponent(EmptyRoot);

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(GetRootComponent());
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonMesh->SetCastShadow(false);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetReceivesDecals(false);

	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("ThirdPersonMesh"));
	ThirdPersonMesh->SetupAttachment(GetRootComponent());
	ThirdPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ThirdPersonMesh->SetCastHiddenShadow(true);
	ThirdPersonMesh->SetOwnerNoSee(true);
	ThirdPersonMesh->SetReceivesDecals(false);
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
	return true;
}

bool ABaseWeapon::TryEndFire()
{
	return true;
}

bool ABaseWeapon::TryEquipToParentTransform(const FTransform& Transform, USkeletalMeshComponent* FPMeshAttachment, USkeletalMeshComponent* TPMeshAttachment, FName SocketName)
{
	if ((EquippedTransform = &Transform) == nullptr) { return false; }

	FirstPersonMesh->AttachToComponent(FPMeshAttachment, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	ThirdPersonMesh->AttachToComponent(TPMeshAttachment, FAttachmentTransformRules::KeepRelativeTransform, SocketName);

	return true;
}

float ABaseWeapon::GetFalloffAdjustedDamage(float Distance) const
{
	return BaseDamage * (1000 / Distance);
}

USkeletalMeshComponent* ABaseWeapon::GetVisibleMesh() const
{
	TObjectPtr<APawn> WeaponOwner = GetOwner<APawn>();
	if (!IsValid(WeaponOwner)) { return nullptr; }

	if (WeaponOwner->IsPlayerControlled())
	{
		return FirstPersonMesh;
	}
	else
	{
		return ThirdPersonMesh;
	}
}

bool ABaseWeapon::TryDamageDamageable(TScriptInterface<IDamageable> Damageable, FVector DamageStartLocation, float Distance)
{
	TObjectPtr<ABaseCharacter> BaseCharacter = GetOwner<ABaseCharacter>();
	if (!IsValid(BaseCharacter)) { return false; }

	TObjectPtr<APawn> DamageablePawn = Cast<APawn>(Damageable.GetInterface());
	if (!IsValid(DamageablePawn)) { return false; }
	
	if (BaseCharacter->IsPlayerControlled() == DamageablePawn->IsPlayerControlled())
	{
		return false;
	}

	IDamageable::Execute_OnDamaged(DamageablePawn, GetFalloffAdjustedDamage(Distance), DamageStartLocation, BaseCharacter->CharacterColour);

	return true;
}

UAnimSequence* ABaseWeapon::GetArmedAnimSequence_Implementation() const
{
	return ArmedAnimSequence;
}

UAimOffsetBlendSpace* ABaseWeapon::GetArmedAimOffset_Implementation() const
{
	return ArmedAimOffset;
}
