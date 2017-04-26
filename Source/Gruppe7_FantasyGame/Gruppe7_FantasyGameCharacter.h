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
	// Materials for cursor.

	UPROPERTY(EditAnywhere, Category = "Cursor Materials")
		UMaterialInterface* Water;

	UPROPERTY(EditAnywhere, Category = "Cursor Materials")
		UMaterialInterface* Fire;

	UPROPERTY(EditAnywhere, Category = "Cursor Materials")
		UMaterialInterface* Thorns;

	UPROPERTY(EditAnywhere, Category = "Cursor Materials")
		UMaterialInterface* Healing;

	///////////////////////////////////////////////////////////////////////////
	// Attack object. - Physical hit box and magic effects.

	// Selects magic projectile.
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AMagicProjectile> MagicProjectileBlueprint;

	// Selects cone of fire.
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AConeOfFire> MagicFireConeBlueprint;

	// Selects circle of fire.
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ACircleOfThorns> MagicThornCircleBlueprint;

	// Selects physical hitbox.
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class APhysAttackBox> PhysAttackBlueprint;

	// Selects knockback sphere.
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AKnockbackSphere> KnockbackBlueprint;

	///////////////////////////////////////////////////////////////////////////
	// Health and Mana variables.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD Variables")
		float Mana;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD Variables")
		float Health;

	// Overlap function.
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// Handles the player characters death.
		void Respawner();

	// Functions that deal with Sound FX
		int RandomInt;

		void PlayerDamageSound();

		void PlayerAttackSound();

	// SFX.
	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* PickUpSound;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* AttackSound01;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* AttackSound02;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* YellSound01;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* YellSound02;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* DamageSound01;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* DamageSound02;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* PainSound01;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* PainSound02;

	// VFX.
	UPROPERTY(EditAnywhere, Category = "VFX")
		UParticleSystem* HealthPickUpFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UParticleSystem* ManaPickUpFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UParticleSystem* HitFX;


protected:
	
	///////////////////////////////////////////////////////////////////////////
	// Respawn related variables.
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Levels")
		int CurrentLevel;

	///////////////////////////////////////////////////////////////////////////
	// Spell select variables.
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD Variables")
		int SpellSelect;


	///////////////////////////////////////////////////////////////////////////
	// Movement Variables.

	UPROPERTY(EditAnywhere)
		float PlayerMovementSpeed = 600.f;

	///////////////////////////////////////////////////////////////////////////
	// Power-Up Variables.

	/* Handle for power-up timer */
	FTimerHandle SpeedPowerUpTimerHandle;

	bool PlayerHasPowerup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
		float SpeedPowerUpDuration = 10.f;

	/** Called for forward/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called for attacks */
	void PhysAttack();

	void MagiProjectile();

	void MagiFireCone();

	void MagiThornCircle();

	void MagiHealing();

	void MagiAttack();

	void MagiAttackStop();

	bool SpellIsContinuous = false;

	bool AttackDelay = false;

	bool MagicDelay = false;

	// Attack/Spell timers.
	
	float SpellContTimer = 0.f;

	float SpellDelay = 30.f;

	float AttackTimer = 0.f;

	float MagicTimer = 0.f;


	/** Item functions */
	void ManaPotion(float ManaRestore);

	void HealthPotion(float HealthRestore);

	void PowerUp_Speed();

	void PowerUp_SpeedOver();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Tick(float DeltaSeconds);

	// Spell switching.
	void SpellSwapUp();

	void SpellSwapDown();

	void SpellSwap(bool SwapUp);

	void SelectWater();

	void SelectFire();

	void SelectThorns();

	void SelectHealing();

	// End of APawn interface

public:
	///** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return TopDownCameraComponent; }
	/** A decal that projects to the cursor location. */
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
};

