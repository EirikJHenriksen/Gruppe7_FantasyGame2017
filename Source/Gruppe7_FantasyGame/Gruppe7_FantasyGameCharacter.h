// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Gruppe7_FantasyGameCharacter.generated.h"

UCLASS(config=Game)
class AGruppe7_FantasyGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;
public:
	AGruppe7_FantasyGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	///////////////////////////////////////////////////////////////////////////
	// Attack object. - Physical hit box and magic effects.

	// Selects magic projectile.
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AMagicProjectile> MagicProjectileBlueprint;

	// Selects cone of fire.
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AConeOfFire> MagicFireConeBlueprint;

	// Selects physical hitbox.
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class APhysAttackBox> PhysAttackBlueprint;

	///////////////////////////////////////////////////////////////////////////
	// Health and Mana variables.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD Variables")
		float Mana = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD Variables")
		float Health = 1.f;



	// Overlap function.
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// VFX.
	//UPROPERTY(EditAnywhere)
		//UParticleSystem *PickUpFX;

	// SFX.
	//UPROPERTY(EditAnywhere)
		//USoundBase* PickUpSound;
		


protected:
	///////////////////////////////////////////////////////////////////////////
	// Spell selevt variables.
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD Variables")
		int SpellSelect;


	///////////////////////////////////////////////////////////////////////////
	// Movement Variables.

	UPROPERTY(EditAnywhere)
		float PlayerMovementSpeed = 600.f;

	///////////////////////////////////////////////////////////////////////////
	// Power-Up Variables.

	bool PlayerHasPowerup;

	float MasterTimer;

	float PickUpTime;

	float DurationTimer;


	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called for attacks */
	void PhysAttack();

	void MagiProjectile();

	void MagiFireCone();

	void MagiAttack();

	void ManaPotion();

	void HealthPotion();

	void PowerUp_Speed();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Tick(float DeltaSeconds);
	void SpellSwapUp();
	void SpellSwapDown();
	void SpellSwap(bool SwapUp);
	void SpellSwap();
	// End of APawn interface

public:
	///** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return TopDownCameraComponent; }
	/** A decal that projects to the cursor location. */
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
};

