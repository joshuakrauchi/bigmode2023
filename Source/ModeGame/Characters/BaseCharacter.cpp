#include "BaseCharacter.h"
#include "Interfaces/Fireable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerControllers/GameplayPC.h"
#include "../GameStates/GameplayGS.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetRootComponent());
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetCastShadow(false);
	FirstPersonMesh->SetOnlyOwnerSee(true);

	TObjectPtr<USkeletalMeshComponent> ThirdPersonMesh = GetMesh();
	if (IsValid(ThirdPersonMesh))
	{
		ThirdPersonMesh->SetCastHiddenShadow(true);
		ThirdPersonMesh->SetOwnerNoSee(true);
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

	CurrentHealth = BaseHealth;

	SpawnFireable();
}

// Called when the game stops or when despawned
void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TObjectPtr<AActor> Weapon = GetFireableActor();
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

void ABaseCharacter::OnDamaged_Implementation(float DamageAmount)
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	if (IsPlayerControlled())
	{
		TObjectPtr<AGameplayGS> GameplayGS = World->GetGameState<AGameplayGS>();
		if (IsValid(GameplayGS))
		{
			GameplayGS->DecreaseHealth(DamageAmount);
		}
	}
	else
	{
		CurrentHealth -= DamageAmount;

		if (CurrentHealth <= 0.0f)
		{
			OnHealthDepleted();
		}
	}
}

USkeletalMeshComponent* ABaseCharacter::GetFirstPersonMesh() const
{
	return FirstPersonMesh;
}

bool ABaseCharacter::HasFireable() const
{
	return (IsValid(FireableActor));
}

AActor* ABaseCharacter::GetFireableActor() const
{
	return FireableActor;
}

void ABaseCharacter::SetFireableActor(AActor* Actor)
{
	FireableActor = Actor;
}

UCameraComponent* ABaseCharacter::GetFirstPersonCamera() const
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

void ABaseCharacter::SpawnFireable()
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	if (!IsValid(FireableClass)) { return; }
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	TObjectPtr<AActor> SpawnedFireableActor = World->SpawnActor<AActor>(FireableClass, FTransform(), SpawnParams);
	SetFireableActor(SpawnedFireableActor);

	UCameraComponent* Camera = GetFirstPersonCamera();
	IFireable* Fireable = Cast<IFireable>(SpawnedFireableActor);
	if (Fireable != nullptr && IsValid(Camera))
	{
		Fireable->TryEquipToParentTransform(Camera->GetComponentTransform(), FirstPersonMesh, GetMesh(), FName("weapon_r"));
	}
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

void ABaseCharacter::OnHealthDepleted()
{
	Destroy();
}
