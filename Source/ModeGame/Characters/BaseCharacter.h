#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "BaseCharacter.generated.h"

class IFireable;
class UCameraComponent;
class USkeletalMeshComponent;

UENUM()
enum class ECharacterType : uint8
{
	None,

	Grey,
	Red,
	Green,
	Blue,

	Count,
};

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
	virtual void Tick(float DeltaTime) override;

	virtual void OnDamaged_Implementation(float DamageAmount) override;

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

private:
	UFUNCTION()
		void SpawnFireable();

	UFUNCTION()
		void UpdateResetDoubleJump();

	UFUNCTION()
		void OnHealthDepleted();

};
