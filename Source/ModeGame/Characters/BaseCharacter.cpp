#include "BaseCharacter.h"
#include "Interfaces/Fireable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerControllers/GameplayPC.h"

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
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TObjectPtr<UCharacterMovementComponent> Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = 900.0f;
	Movement->JumpZVelocity = 600.0f;
	Movement->AirControl = 1.0f;
	Movement->AirControlBoostMultiplier = 100.0f;
	Movement->SetWalkableFloorAngle(90.0f);
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

	IFireable* Weapon = GetFireable();
	UCameraComponent* Camera = GetFirstPersonCamera();
	if (Weapon != nullptr && IsValid(Camera))
	{
		Weapon->TryEquipToParentTransform(Camera->GetComponentTransform());
	}
}

// Called when the game stops or when despawned
void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	IFireable* WeaponFireable = GetFireable();
	AActor* Weapon = Cast<AActor>(WeaponFireable);
	if (IsValid(Weapon))
	{
		Weapon->Destroy();
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateResetDoubleJump();
}

USkeletalMeshComponent* ABaseCharacter::GetFirstPersonMesh()
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

void ABaseCharacter::SetFireableActor(AActor* Actor)
{
	FireableActor = Actor;
}

UCameraComponent* ABaseCharacter::GetFirstPersonCamera()
{
	return FirstPersonCamera;
}

void ABaseCharacter::ResetDoubleJump()
{
	bCanJumpAgain = true;
}

bool ABaseCharacter::CanDoubleJump() const
{
	return bCanJumpAgain;
}

void ABaseCharacter::DoubleJump()
{
	TObjectPtr<AGameplayPC> GameplayPC = GetController<AGameplayPC>();
	if (!IsValid(GameplayPC)) { return; }

	FVector ImpulseVector{ -GameplayPC->GetMoveInput().X, GameplayPC->GetMoveInput().Y, 0.0f };
	ImpulseVector *= DoubleJumpImpulseStrength;

	FRotator Rotation = GetControlRotation();
	Rotation.Yaw += -90.0f;

	ImpulseVector = Rotation.RotateVector(ImpulseVector);
	ImpulseVector.Z = DoubleJumpHeight;

	TObjectPtr<UCharacterMovementComponent> Movement = GetCharacterMovement();
	if (!IsValid(Movement)) { return; }

	Movement->Velocity = ImpulseVector;

	bCanJumpAgain = false;
}

void ABaseCharacter::UpdateResetDoubleJump()
{
	TObjectPtr<UCharacterMovementComponent> Movement = GetCharacterMovement();
	if (!IsValid(Movement)) { return; }

	if (!Movement->IsFalling())
	{
		ResetDoubleJump();
	}
}
