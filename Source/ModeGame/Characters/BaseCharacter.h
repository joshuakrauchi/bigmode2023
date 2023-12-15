#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "Materials/MaterialParameterCollection.h"
#include "BaseCharacter.generated.h"

class IFireable;
class UCameraComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;
class UMaterialParameterCollection;

UCLASS()
class MODEGAME_API ABaseCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UCameraComponent> FirstPersonCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<USkeletalMeshComponent> FirstPersonMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UClass> FireableClass = nullptr;
	
	UPROPERTY(EditAnywhere)
		float BaseHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EPlayableColours CharacterColour = EPlayableColours::None;

	UPROPERTY(EditAnywhere)
		float SameColourDamageMultiplier = 0.0f;

	UPROPERTY(EditAnywhere)
		float StrongAgainstColourDamageMultiplier = 0.0f;

	UPROPERTY(EditAnywhere)
		float WeakAgainstColourDamageMultiplier = 0.0f;

	UPROPERTY(EditAnywhere)
		float HardStrongAgainstColourDamageMultiplier = 0.0f;

	UPROPERTY(EditAnywhere)
		float HardWeakAgainstColourDamageMultiplier = 0.0f;

	UPROPERTY(EditAnywhere)
		float BaseInvincibilityTime = 0.0f;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UTextRenderComponent> ScoreTextRenderComponent = nullptr;

	UPROPERTY(EditAnywhere)
		float ScoreTextRiseSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
		float ScoreTextBaseSizeAmount = 0.0f;

	UPROPERTY(EditAnywhere)
		float ScoreTextBaseSize = 0.0f;

	UPROPERTY(EditAnywhere)
		float ScoreTextMaxSize = 0.0f;

	UPROPERTY(EditAnywhere)
		float ScoreTextMinSize = 0.0f;

	UPROPERTY(EditAnywhere)
		float BaseTimeBeforeScoreTextDisappear = 0.0f;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UClass> CharacterToSpawnOnDeath = nullptr;

	UPROPERTY(BlueprintReadWrite)
		bool bIsPlayingDamagedMontage = false;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UMaterialParameterCollection> GameColors = nullptr;

	UPROPERTY(BlueprintReadWrite)
		FVector SafeSpawnerLocation;

	UPROPERTY(BlueprintReadWrite)
		bool bIsMovingToSafeSpawner = false;

private:
	UPROPERTY()
		TObjectPtr<AActor> FireableActor = nullptr;

	UPROPERTY()
		bool bCanJumpAgain = false;

	UPROPERTY(EditAnywhere)
		float DoubleJumpImpulseStrength = 1100.0f;

	UPROPERTY(EditAnywhere)
		float DoubleJumpHeight = 600.0f;

	UPROPERTY()
		float CurrentHealth = 0;

	UPROPERTY()
		float CurrentInvincibilityTime = 0.0f;

	UPROPERTY()
		float CurrentTimeBeforeScoreTextDisappear = 0.0f;

	UPROPERTY(EditAnywhere)
		float SafeSpawnerMoveSpeed = 0.0f;

	UPROPERTY()
		float TotalDamagedSinceLastFrame;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game stops or when despawned
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnDamaged_Implementation(float DamageAmount, FVector DamageStartLocation, EPlayableColours SourceColour = EPlayableColours::None) override;

	UFUNCTION(BlueprintCallable)
		USkeletalMeshComponent* GetFirstPersonMesh() const;

	UFUNCTION(BlueprintCallable)
		bool HasFireable() const;

	UFUNCTION(BlueprintCallable)
		AActor* GetFireableActor() const;

	UFUNCTION(BlueprintCallable)
		void SetFireableActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
		UCameraComponent* GetFirstPersonCamera() const;

	UFUNCTION(BlueprintCallable)
		void ResetDoubleJump();

	UFUNCTION(BlueprintCallable)
		bool CanDoubleJump() const;

	UFUNCTION(BlueprintCallable)
		void DoubleJump();

	UFUNCTION(BlueprintCallable)
		bool IsExhausted() const;

	UFUNCTION(BlueprintCallable)
		bool CanBePossessed() const;

	UFUNCTION(BlueprintCallable)
		FVector GetProjectileStartLocation() const;

	UFUNCTION(BlueprintCallable)
		FVector GetProjectileEndLocation(float Range, float ScatterRange) const;

	UFUNCTION()
		bool IsInvincible() const;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayDamagedPlayerSFX();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayDamagedAISFX();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayDoubleJumpSFX();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayDamagedMontage();

	UFUNCTION()
		bool IsPlayingDamagedMontage() const;

	UFUNCTION()
		FLinearColor GetColorFromCollection(EPlayableColours Colour) const;

	UFUNCTION()
		void UpdateMoveToSafeSpawner(float DeltaSeconds);

	UFUNCTION(BlueprintImplementableEvent)
		void CreateDamageIndicator(FVector DamageStartLocation, FLinearColor DamageColor);

	UFUNCTION(BlueprintCallable)
		void DestroyAIController();

private:
	UFUNCTION()
		void SpawnFireable();

	UFUNCTION()
		void UpdateResetDoubleJump();

	UFUNCTION()
		void OnHealthDepleted();

	UFUNCTION()
		float GetIncomingDamageMultiplierForColour(EPlayableColours IncomingDamageColour);

	UFUNCTION()
		void UpdateInvincibility(float DeltaSeconds);

	UFUNCTION()
		void SetScoreText(int Number, EPlayableColours Colour);

	UFUNCTION()
		void UpdateScoreText(float DeltaSeconds);

	UFUNCTION()
		void SpawnExplodingDeathCharacter();

};
