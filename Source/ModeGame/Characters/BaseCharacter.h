#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class IFireable;

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

private:
	UPROPERTY()
		TObjectPtr<AActor> FireableActor = nullptr;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	IFireable* GetFireable();

protected:

};
