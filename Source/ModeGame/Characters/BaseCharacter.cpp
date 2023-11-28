#include "BaseCharacter.h"
#include "../Interfaces/Fireable.h"
#include "Camera/CameraComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetRootComponent());
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetCastShadow(false);

	TObjectPtr<USkeletalMeshComponent> ThirdPersonMesh = GetMesh();
	if (IsValid(ThirdPersonMesh))
	{
		ThirdPersonMesh->SetCastHiddenShadow(true);
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(GetFirstPersonMesh()))
	{
		GetFirstPersonMesh()->SetHiddenInGame(!bIsFirstPersonMode);
	}

	USkeletalMeshComponent* ThirdPersonMesh = GetMesh();
	if (IsValid(ThirdPersonMesh))
	{
		ThirdPersonMesh->SetHiddenInGame(bIsFirstPersonMode);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TObjectPtr<USkeletalMeshComponent> ABaseCharacter::GetFirstPersonMesh()
{
	return FirstPersonMesh;
}

bool ABaseCharacter::IsFirstPersonMode() const
{
	return bIsFirstPersonMode;
}

void ABaseCharacter::SetFirstPersonMode(bool bFirstPersonMode)
{
	bIsFirstPersonMode = bFirstPersonMode;
}

IFireable* ABaseCharacter::GetFireable()
{
	return Cast<IFireable>(FireableActor);
}
