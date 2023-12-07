// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EmptyRoot = CreateDefaultSubobject<USceneComponent>(FName("EmptyRoot"));
	SetRootComponent(EmptyRoot);

	FPMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("FPMesh"));
	FPMesh->SetupAttachment(GetRootComponent());
	FPMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FPMesh->SetCastShadow(false);
	FPMesh->SetOnlyOwnerSee(true);

	TPMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("TPMesh"));
	TPMesh->SetupAttachment(GetRootComponent());
	TPMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TPMesh->SetCastHiddenShadow(true);
	TPMesh->SetOwnerNoSee(true);
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

bool ABaseWeapon::TryEquipToParentTransform(const FTransform& Transform, USkeletalMeshComponent* FPMeshAttachment, USkeletalMeshComponent* TPMeshAttachment, FName SocketName)
{
	if ((EquippedTransform = &Transform) == nullptr) { return false; }

	FPMesh->AttachToComponent(FPMeshAttachment, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	TPMesh->AttachToComponent(TPMeshAttachment, FAttachmentTransformRules::KeepRelativeTransform, SocketName);

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
