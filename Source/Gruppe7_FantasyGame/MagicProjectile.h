// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MagicProjectile.generated.h"

UCLASS()
class GRUPPE7_FANTASYGAME_API AMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	// What happens when destroyed.
	void Destroy();

	// Overlap function
	//void OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Variables
	FVector CurrentVelocity;

	// Components that can be set/changed in editor.
	UPROPERTY(EditAnywhere)
		UShapeComponent* RootSphere;

	//UPROPERTY(EditAnywhere)
		//USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
		float Speed = 1000.f;

	UPROPERTY(EditAnywhere)
		float TimeBeforeDestroy = 20.f;

		float TimeLived{ 0 };

	// VFX.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFXs")
		UParticleSystem *ImpactSparkFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFXs")
		UParticleSystem *MagiSpark;
};
