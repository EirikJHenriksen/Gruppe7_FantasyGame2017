// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FinalBoss.generated.h"

UCLASS()
class GRUPPE7_FANTASYGAME_API AFinalBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFinalBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/////////////////////////////////////////
	// LOCATION VARIABLES
	void UpdateDirection();

	FVector CurrentPlayerLocation;

	FVector LookVector;

	/////////////////////////////////////////
	// TELEPORTATION VARIABLES.
	void Teleport();

	bool fightInProgress;

	bool canTeleport;

	FTimerHandle TeleportTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport timers")
		float RandomMin = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport timers")
		float RandomMax = 10;

	float RandomTeleportTime = 0;

	// Teleport locations.
	
	// FIRE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport FIRE")
		float FireX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport FIRE")
		float FireY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport FIRE")
		float FireZ = 0;

	// WATER
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport WATER")
		float WaterX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport WATER")
		float WaterY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport WATER")
		float WaterZ = 0;

	// NATURE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport NATURE")
		float NatureX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport NATURE")
		float NatureY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport NATURE")
		float NatureZ = 0;

	// Overlap function
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
};