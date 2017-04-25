// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BossSpellFire.generated.h"

UCLASS()
class GRUPPE7_FANTASYGAME_API ABossSpellFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossSpellFire();

	// Velocity
	FVector CurrentVelocity;

	/////////////////////////////////////////
	// LOCATION VARIABLES
	void UpdateTarget();

	FVector CurrentPlayerLocation;

	FVector TargetVector;

	FVector VelocityVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// What happens when destroyed.
	void Destroy();

	// Sets up collision.
	UPROPERTY(EditAnywhere)
		USphereComponent* CollisionComponent;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
