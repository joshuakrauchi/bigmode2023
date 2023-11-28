#include "BaseCharacter.h"
#include "../Interfaces/Fireable.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

IFireable* ABaseCharacter::GetFireable()
{
	return Cast<IFireable>(FireableActor);
}
