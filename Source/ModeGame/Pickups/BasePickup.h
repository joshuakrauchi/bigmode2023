// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class MODEGAME_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USceneComponent> EmptyRoot = nullptr;
	
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USphereComponent> Collision = nullptr;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UNiagaraComponent> ParticleComponent = nullptr;

public:	
	// Sets default values for this actor's properties
	ABasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual bool TryApplyPlayerPickupEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayPickedUpSFX();

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
