// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/BasePickup.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

// Sets default values
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EmptyRoot = CreateDefaultSubobject<USceneComponent>(FName("EmptyRoot"));
	SetRootComponent(EmptyRoot);

	Collision = CreateDefaultSubobject<USphereComponent>(FName("Collision"));
	Collision->SetupAttachment(GetRootComponent());

	ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("ParticleComponent"));
	ParticleComponent->SetupAttachment(Collision);
}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABasePickup::OnComponentBeginOverlap);
}

bool ABasePickup::TryApplyPlayerPickupEffect()
{
	PlayPickedUpSFX();

	return true;
}

void ABasePickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<APawn> OtherPawn = Cast<APawn>(OtherActor);
	if (!IsValid(OtherPawn)) { return; }

	if (!OtherPawn->IsPlayerControlled()) { return; }

	if (TryApplyPlayerPickupEffect())
	{
		Destroy();
	}
}
