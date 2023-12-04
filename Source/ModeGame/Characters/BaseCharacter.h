#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
class MODEGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		bool bIsFirstPersonMode = false;

private:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UCameraComponent> FirstPersonCamera = nullptr;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USkeletalMeshComponent> FirstPersonMesh = nullptr;

	UPROPERTY()
		TObjectPtr<AActor> FireableActor = nullptr;

	UPROPERTY()
		bool bCanJumpAgain = false;

	UPROPERTY(EditAnywhere)
		float DoubleJumpImpulseStrength = 1100.0f;

	UPROPERTY(EditAnywhere)
		float DoubleJumpHeight = 600.0f;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		USkeletalMeshComponent* GetFirstPersonMesh();

	UFUNCTION(BlueprintCallable)
		bool IsFirstPersonMode() const;

	UFUNCTION(BlueprintCallable)
		void SetFirstPersonMode(bool bFirstPersonMode);

	IFireable* GetFireable();

	UFUNCTION(BlueprintCallable)
		void SetFireableActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
		UCameraComponent* GetFirstPersonCamera();

	UFUNCTION(BlueprintCallable)
		void ResetDoubleJump();

	UFUNCTION(BlueprintCallable)
		bool CanDoubleJump() const;

	UFUNCTION(BlueprintCallable)
		void DoubleJump();

protected:

};
