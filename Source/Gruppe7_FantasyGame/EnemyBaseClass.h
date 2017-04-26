// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyBaseClass.generated.h"

// An enum class that hold the kinds of elements enemies can belong to 
UENUM(BlueprintType) //"BlueprintType" is essential to include
enum class ElementsEnum : uint8
{
	FIRE,
	WATER,
	NATURE 
};

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


	// Mana?
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AManaPotion> SpawningMana; // holds the blueprint of the mana potion

	/////////////////////////////////////
	// VFX.

	UPROPERTY(EditAnywhere, Category = "VFX")
		UParticleSystem *HitFX;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MeleeAttack();

	// getters
	float GetDistanceToPlayer();

	FVector GetMyStartLocation();

	// sets DistanceToPlayer
	void UpdateDistance();

	//// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Overlap function.
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void DeathCheck();

	void EnemyIsHit(float force, float duration);

	void SlowdownOver();

	// Should safely remove this at some point...
	UPROPERTY(EditAnywhere, Category = Behavior)
		class UBehaviorTree *BotBehavior;

	// Distance detection variables.
	FVector CurrentPlayerLocation;
	FVector DistanceVector;
	float DistanceFloat;

	// Elements for enemie
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		ElementsEnum Element;

	bool CanSeePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/////////////////
	// Timers for attack.
	float AttackTimer = 30.f;

	/* Handle for slowdown timer */
	FTimerHandle SlowdownTimerHandle;

private:
	// enemies HP
	UPROPERTY(EditAnywhere)
		float HealthPoints = 100.f;

	// saves where player starts, used to go back "home"
	UPROPERTY(EditAnywhere)
		FVector MyStartLocation;

	UPROPERTY(EditAnywhere)
		float MovementSpeed = 500.f;

	// holds the distance between enemy and player
	float DistanceToPlayer = 1000.f; // just initializing a arbitrary value

	// How much different attacks hurt
	float DamageMelee = 20.f;
	// these are used depending on the enemies element type and the magic attacks element type
	float DamageMyWeakness = 60.f;
	float DamageMyEqual = 25.f;
	float DamageLeastEffective = 10.f;

	// Sounds
	UPROPERTY(EditAnywhere)
		USoundBase* EnemyDeathSound;

	UPROPERTY(EditAnywhere)
		USoundBase* EnemyHurtSound;

	UPROPERTY(EditAnywhere)
		USoundBase* EnemyMeleeAttackSound;



	//typedef void (AEnemyBaseClass::*FunctionPointer)(void);
	//trying to make a function pointer for states, basically
	//FunctionPointer (*AIFunctionPointer)() = &StateIdle;
/*
	typedef void(AEnemyBaseClass::*AIFunctionPointerType)(void);
	AIFunctionPointerType AIFunction = &AEnemyBaseClass::StateIdle;*/
};
