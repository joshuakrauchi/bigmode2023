#include "BaseCharacter.h"
#include "Interfaces/Fireable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerControllers/GameplayPC.h"
#include "GameStates/GameplayGS.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Text/ScoreNumber.h"
#include "Components/TextRenderComponent.h"

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

	ScoreTextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(FName("ScoreTextRenderComponent"));
	ScoreTextRenderComponent->SetupAttachment(GetRootComponent());
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
void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateResetDoubleJump();
	UpdateInvincibility(DeltaSeconds);
	UpdateScoreText(DeltaSeconds);
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	IFireable* Fireable = Cast<IFireable>(GetFireableActor());
	if (Fireable != nullptr)
	{
		Fireable->TryEndFire();
	}
}

void ABaseCharacter::OnDamaged_Implementation(float DamageAmount, EPlayableColours SourceColour)
{
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	TObjectPtr<AGameplayGS> GameplayGS = World->GetGameState<AGameplayGS>();
	if (!IsValid(GameplayGS)) { return; }

	float TotalDamage = (DamageAmount * GetIncomingDamageMultiplierForColour(SourceColour));

	if (!IsPlayerControlled())
	{
		// Add score regardless of invincibility.
		int ScoreText = FMath::RoundFromZero(TotalDamage);

		SetScoreText(ScoreText, SourceColour);
		
		GameplayGS->AddScore(ScoreText);
	}

	if (IsInvincible()) { return; }

	if (IsPlayerControlled())
	{
		GameplayGS->DecreaseHealth(DamageAmount);
	}
	else
	{
		bool bWasExhausted = IsExhausted();

		CurrentHealth -= DamageAmount;

		if (IsExhausted() && !bWasExhausted)
		{
			CurrentInvincibilityTime = BaseInvincibilityTime;
		}
		else if (CurrentHealth <= 0.0f)
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

bool ABaseCharacter::IsExhausted() const
{
	return (!IsPlayerControlled() && (CurrentHealth <= (BaseHealth * ExhaustedHealthMultiplierThreshold)));
}

bool ABaseCharacter::CanBePossessed() const
{
	return IsExhausted();
}

FVector ABaseCharacter::GetProjectileStartLocation() const
{
	TObjectPtr<UCameraComponent> Camera = GetFirstPersonCamera();
	if (!IsValid(Camera)) { return FVector::Zero(); }

	return (Camera->GetForwardVector() + Camera->GetComponentLocation());
}

FVector ABaseCharacter::GetProjectileEndLocation(float Range, float ScatterRange) const
{
	FVector Movement = UKismetMathLibrary::GetForwardVector(GetControlRotation());

	if (ScatterRange > 0.0f)
	{
		float HalfRange = (ScatterRange / 2.0f);
		FRotator RandomRotation = FRotator(FMath::RandRange(-HalfRange, HalfRange), FMath::RandRange(-HalfRange, HalfRange), 0.0f);
		Movement = RandomRotation.RotateVector(Movement);
	}

	return ((Movement * Range) + GetActorLocation());
}

bool ABaseCharacter::IsInvincible() const
{
	return (CurrentInvincibilityTime > 0.0f);
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

float ABaseCharacter::GetIncomingDamageMultiplierForColour(EPlayableColours IncomingDamageColour)
{
	if (CharacterColour == IncomingDamageColour)
	{
		return SameColourDamageMultiplier;
	}

	switch (CharacterColour)
	{
	case EPlayableColours::Red:
	{
		if (IncomingDamageColour == EPlayableColours::Blue)
		{
			return WeakAgainstColourDamageMultiplier;
		}
		else if (IncomingDamageColour == EPlayableColours::Green)
		{
			return StrongAgainstColourDamageMultiplier;
		}
	}
	break;
	case EPlayableColours::Green:
	{
		if (IncomingDamageColour == EPlayableColours::Red)
		{
			return WeakAgainstColourDamageMultiplier;
		}
		else if (IncomingDamageColour == EPlayableColours::Blue)
		{
			return StrongAgainstColourDamageMultiplier;
		}
	}
	break;
	case EPlayableColours::Blue:
		if (IncomingDamageColour == EPlayableColours::Green)
		{
			return WeakAgainstColourDamageMultiplier;
		}
		else if (IncomingDamageColour == EPlayableColours::Red)
		{
			return StrongAgainstColourDamageMultiplier;
		}
		break;
	case EPlayableColours::Grey:
	case EPlayableColours::None:
	case EPlayableColours::Count:
	default:
		break;
	}

	return 1.0f;
}

void ABaseCharacter::UpdateInvincibility(float DeltaSeconds)
{
	if (!IsInvincible()) { return; }

	CurrentInvincibilityTime -= DeltaSeconds;
}

void ABaseCharacter::UpdateScoreText(float DeltaSeconds)
{
	if (!IsValid(ScoreTextRenderComponent)) { return; }

	if (!ScoreTextRenderComponent->IsVisible()) { return; }

	CurrentTimeBeforeScoreTextDisappear -= DeltaSeconds;
	if (CurrentTimeBeforeScoreTextDisappear <= 0.0f)
	{
		ScoreTextRenderComponent->SetVisibility(false);
	}
	
	TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) { return; }

	TObjectPtr<ACharacter> PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!IsValid(PlayerCharacter)) { return; }

	ScoreTextRenderComponent->AddRelativeLocation(GetActorUpVector() * ScoreTextRiseSpeed * DeltaSeconds);
	ScoreTextRenderComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(ScoreTextRenderComponent->GetComponentLocation(), PlayerCharacter->GetActorLocation()));
}

void ABaseCharacter::SetScoreText(int Number, EPlayableColours Colour)
{
	if (!IsValid(ScoreTextRenderComponent)) { return; }

	ScoreTextRenderComponent->SetRelativeLocation(UKismetMathLibrary::RandomUnitVector() * 50.0f);

	FColor RenderColor = FColor::White;
	switch (Colour)
	{
	case EPlayableColours::Grey:
	{
		RenderColor = FColor(128.0f, 128.0f, 128.0f);
	}
	break;
	case EPlayableColours::Red:
	{
		RenderColor = FColor::Red;
	}
	break;
	case EPlayableColours::Green:
	{
		RenderColor = FColor::Green;
	}
	break;
	case EPlayableColours::Blue:
	{
		RenderColor = FColor::Blue;
	}
	break;
	case EPlayableColours::None:
	case EPlayableColours::Count:
	default:
		break;
	}

	ScoreTextRenderComponent->SetTextRenderColor(RenderColor);
	ScoreTextRenderComponent->SetText(FText::FromString(FString::Printf(TEXT("+%d"), Number)));
	ScoreTextRenderComponent->SetVisibility(true);
	CurrentTimeBeforeScoreTextDisappear = BaseTimeBeforeScoreTextDisappear;
}
