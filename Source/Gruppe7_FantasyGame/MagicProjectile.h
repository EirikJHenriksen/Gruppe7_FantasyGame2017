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
	//void Destroy();

	// Overlap function
	void OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Variables
	FVector CurrentVelocity;

	// Sets up collision.
	UPROPERTY(EditAnywhere)
		USphereComponent* CollisionComponent;

	//UPROPERTY(EditAnywhere)
		//USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
		float TimeBeforeDestroy = 10.f;

		float TimeLived{ 0 };

	// VFX.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFXs")
		UParticleSystem *ImpactSparkFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFXs")
		UParticleSystem *MagiSpark;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	//UFUNCTION()
		//void OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Function that initializes the projectile's velocity in the shoot direction.
	void FireInDirection(const FVector& ShootDirection);
};
