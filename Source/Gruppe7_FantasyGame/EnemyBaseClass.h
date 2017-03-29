// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyBaseClass.generated.h"

UCLASS()
class GRUPPE7_FANTASYGAME_API AEnemyBaseClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseClass();

	// Selects physical hitbox.
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AEnemyAttackBox> EnemyAttackBlueprint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/////////////////
	// Timer.
	int BadTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Attack();

	void DistanceCheck();

	float GetDistanceToPlayer();

	FVector GetMyStartLocation();

	//// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Overlap function.
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	void DeathCheck();

	UPROPERTY(EditAnywhere, Category = Behavior)
		class UBehaviorTree *BotBehavior;

	///////////////////////////////////////
	// Distance detection variables.
	FVector TargetCharacterLoc;

	FVector DistanceVector;

	float DistanceFloat;

	float EngageRange = 500.f;

	float AttackRange = 100.f;

	bool EngageTarget = false;

	bool AttackTarget = false;

private:
	UPROPERTY(EditAnywhere)
		float HealthPoints = 100.f;

	UPROPERTY(EditAnywhere)
		FVector MyStartLocation;
};
