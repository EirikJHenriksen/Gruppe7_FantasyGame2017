// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Gruppe7_FantasyGame.h"
#include "MagicProjectile.h"
#include "ConeOfFire.h"
#include "CircleOfThorns.h"
#include "PhysAttackBox.h"
#include "KnockbackSphere.h"
#include "ManaPotion.h"
#include "HealthPotion.h"
#include "PowerUp_Speed.h"
#include "WizardCloud.h"
#include "EnemyAttackBox.h"
#include "FantasyGameInstance.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Gruppe7_FantasyGameCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AGruppe7_FantasyGameCharacter

AGruppe7_FantasyGameCharacter::AGruppe7_FantasyGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCapsuleComponent()->bGenerateOverlapEvents = true;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGruppe7_FantasyGameCharacter::OnOverlap);

	// set our turn rates for input
	BaseTurnRate = 0.f;
	BaseLookUpRate = 0.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	true = default.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does. true = default.
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(0.f, 0.f, 90.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level - false = default.

	// Create a follow camera
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Materials/Pointers/PawDecal_MAT.PawDecal_MAT'"));
	//
	//if (DecalMaterialAsset.Succeeded())
	//{
	//	CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	//}
	
	CursorToWorld->DecalSize = FVector(32.0f, 64.0f, 64.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGruppe7_FantasyGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Magic and physical attacks.
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::PhysAttack);
	PlayerInputComponent->BindAction("CastSpell", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::MagiAttack);
	PlayerInputComponent->BindAction("CastSpell", IE_Released, this, &AGruppe7_FantasyGameCharacter::MagiAttackStop);
	PlayerInputComponent->BindAction("SpellSwapUp", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::SpellSwapUp);
	PlayerInputComponent->BindAction("SpellSwapDown", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::SpellSwapDown);

	// Spell selection.
	PlayerInputComponent->BindAction("Water", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::SelectWater);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::SelectFire);
	PlayerInputComponent->BindAction("Thorns", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::SelectThorns);
	PlayerInputComponent->BindAction("Healing", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::SelectHealing);

	// WASD - Movement.
	PlayerInputComponent->BindAxis("MoveForward", this, &AGruppe7_FantasyGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGruppe7_FantasyGameCharacter::MoveRight);
}

void AGruppe7_FantasyGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Updates GameInstance with the players location and velocity.
	Cast<UFantasyGameInstance>(GetGameInstance())->SetPlayerLocation(GetActorLocation());
	//Cast<UFantasyGameInstance>(GetGameInstance())->SetPlayerVelocity(this->GetVelocity());

	// Synchronizes Player variables with game instance.
	Health = Cast<UFantasyGameInstance>(GetGameInstance())->GetHealth();
	Mana = Cast<UFantasyGameInstance>(GetGameInstance())->GetMana();
	SpellSelect = Cast<UFantasyGameInstance>(GetGameInstance())->GetCurrentSpell();

	CurrentLevel = Cast<UFantasyGameInstance>(GetGameInstance())->GetCurrentLevel();

	/////////////////////////
	// Swaps cursor material.
	switch (SpellSelect)
	{
	default:
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("ERROR!!! MATERIAL SWAP FAILED"));
		break;
	case 0:
		CursorToWorld->SetDecalMaterial(Water);
		break;
	case 1:
		CursorToWorld->SetDecalMaterial(Fire);
		break;
	case 2:
		CursorToWorld->SetDecalMaterial(Thorns);
		break;
	case 3:
		CursorToWorld->SetDecalMaterial(Healing);
		break;
	}



	// TIMERS.
	if (SpellIsContinuous)
	{	
		SpellContTimer += 1.f;
		if (SpellContTimer > SpellDelay)
		{
			AGruppe7_FantasyGameCharacter::MagiAttack();
			SpellContTimer = 0.f;
		}
	}

	// Angreps delay.
	if (AttackDelay)
	{
		AttackTimer += 1.f;
		if (AttackTimer > 30.f)
		{
			AttackDelay = false;
			AttackTimer = 0.f;
		}
	}

	// Magi delay.
	if (MagicDelay)
	{
		MagicTimer += 1.f;
		if (MagicTimer > 60.f)
		{
			MagicDelay = false;
			MagicTimer = 0.f;
		}
	}

	FHitResult Hit;
	bool HitResult = false;

	HitResult = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_WorldStatic), true, Hit);

	if (HitResult)
	{
		FVector CursorFV = Hit.ImpactNormal;
		FRotator CursorR = CursorFV.Rotation();
		CursorToWorld->SetWorldLocation(Hit.Location);
		//CursorToWorld->SetWorldRotation(CursorR);
		CursorToWorld->RelativeRotation = FRotator(90.f, 180.f, 90.f);

		FVector CursorLocation = Hit.Location;
		//UE_LOG(LogTemp, Warning, TEXT("Cursor location %s!"), *CursorLocation.ToString());

		FVector TempLocation = FVector(CursorLocation.X, CursorLocation.Y, 30.f);
		FVector NewDirection = TempLocation - GetActorLocation();
		NewDirection.Z = 0.f;
		NewDirection.Normalize();
		SetActorRotation(NewDirection.Rotation());
	}
}

void AGruppe7_FantasyGameCharacter::SpellSwapUp()
{
	AGruppe7_FantasyGameCharacter::SpellSwap(true);
}

void AGruppe7_FantasyGameCharacter::SpellSwapDown()
{
	AGruppe7_FantasyGameCharacter::SpellSwap(false);
}

void AGruppe7_FantasyGameCharacter::SpellSwap(bool SwapUp)
{	
	// Function that changes the active spell.

	if (SwapUp == true)
	{
		Cast<UFantasyGameInstance>(GetGameInstance())->SwapUp();
	}
	if (SwapUp == false)
	{
		Cast<UFantasyGameInstance>(GetGameInstance())->SwapDown();
	}
	
	// DEBUG - Replace with some kind of effect?
	switch (SpellSelect)
	{
	default:
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("ERROR: SPELL SELECT NOT FUNCTIONING!!!"));
		break;
	case 0:
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Magic Projectile Selected"));
		break;
	case 1:
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("Cone of Fire Selected"));
		break;
	case 2:
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("Circle of Fire Selected"));
		break;
	case 3:
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Healing Magic Selected"));
		break;
	case 4:
		//SpellSelect = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Magic Projectile Selected"));
		break;
	case -1:
		//SpellSelect = 3;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Circle of Thorns Selected"));
		break;
	}
}

void AGruppe7_FantasyGameCharacter::SelectWater()
{
	Cast<UFantasyGameInstance>(GetGameInstance())->SelectWater();
}

void AGruppe7_FantasyGameCharacter::SelectFire()
{
	Cast<UFantasyGameInstance>(GetGameInstance())->SelectFire();
}

void AGruppe7_FantasyGameCharacter::SelectThorns()
{
	Cast<UFantasyGameInstance>(GetGameInstance())->SelectThorns();
}

void AGruppe7_FantasyGameCharacter::SelectHealing()
{
	Cast<UFantasyGameInstance>(GetGameInstance())->SelectHealing();
}

void AGruppe7_FantasyGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGruppe7_FantasyGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGruppe7_FantasyGameCharacter::PhysAttack()
{	
	// Legg til delay mellom angrep s� de ikke kan spammes.
	if (!AttackDelay)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AttackDelay = true;

			FVector Location = GetActorLocation();
			FVector Offset = FVector(0.0f, 0.0f, 0.0f);

			FRotator ProjectileRotation = GetActorRotation();

			Location += Offset;

			GetWorld()->SpawnActor<APhysAttackBox>(PhysAttackBlueprint, GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation());

			GetWorld()->SpawnActor<AKnockbackSphere>(KnockbackBlueprint, GetActorLocation(), GetActorRotation());

			PlayerAttackSound();
		}
	}
}

void AGruppe7_FantasyGameCharacter::MagiProjectile()
{	
	if (!MagicDelay)
	{
		//Set the required mana for casting this spell.
		float ManaRequirement{ 0.05f };

		UWorld* World = GetWorld();
		if (World && (Mana >= ManaRequirement))
		{	
			MagicDelay = true;

			FVector Location = GetActorLocation();
			FVector Offset = FVector(50.0f, 0.0f, 0.0f);

			FRotator ProjectileRotation = GetActorRotation();

			Location += Offset;

			GetWorld()->SpawnActor<AMagicProjectile>(MagicProjectileBlueprint, GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation());

			//Spiller skytelyd.
			//UGameplayStatics::PlaySound2D(GetWorld(), CastSound, 1.f, 1.f, 0.f);

			Cast<UFantasyGameInstance>(GetGameInstance())->DrainMana(ManaRequirement);
			//Mana -= ManaRequirement;
		}
	}
}

void AGruppe7_FantasyGameCharacter::MagiFireCone()
{	
	SpellIsContinuous = true;
	SpellDelay = 30.f;

	//Set the required mana for casting this spell.
	float ManaRequirement{ 0.075f };

	UWorld* World = GetWorld();
	if (World && (Mana >= ManaRequirement))
	{
		FVector Location = GetActorLocation();
		FVector Offset = FVector(50.0f, 0.0f, 0.0f);

		FRotator ProjectileRotation = GetActorRotation();

		Location += Offset;

		GetWorld()->SpawnActor<AConeOfFire>(MagicFireConeBlueprint, GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation());

		//Spiller skytelyd.
		//UGameplayStatics::PlaySound2D(GetWorld(), CastSound, 1.f, 1.f, 0.f);

		Cast<UFantasyGameInstance>(GetGameInstance())->DrainMana(ManaRequirement);
	}
}

void AGruppe7_FantasyGameCharacter::MagiThornCircle()
{
	//Set the required mana for casting this spell.
	float ManaRequirement{ 0.1f };

	UWorld* World = GetWorld();
	if (World && (Mana >= ManaRequirement))
	{	
		FVector Location = GetActorLocation();
		FVector Offset = FVector(50.0f, 0.0f, -400.0f);

		FRotator ProjectileRotation = GetActorRotation();

		Location += Offset;

		GetWorld()->SpawnActor<ACircleOfThorns>(MagicThornCircleBlueprint, Location + GetActorForwardVector() * 1.f, GetActorRotation());

		//Spiller skytelyd.
		//UGameplayStatics::PlaySound2D(GetWorld(), CastSound, 1.f, 1.f, 0.f);

		Cast<UFantasyGameInstance>(GetGameInstance())->DrainMana(ManaRequirement);
	}
}

void AGruppe7_FantasyGameCharacter::MagiHealing()
{	
	SpellIsContinuous = true;
	SpellDelay = 1.f;

	// DEBUG.
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("ATTEMPT AT HEALING!!!"));

	if (Health != 1.f && Mana >= 0.f)
	{
		//Set the required mana for casting this spell.
		float ManaRequirement{ 0.02f };
		float HealthRestoration{ 0.01f };

		// DEBUG.
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("HEALING!!!"));

		Cast<UFantasyGameInstance>(GetGameInstance())->RestoreHealth(HealthRestoration);

		Cast<UFantasyGameInstance>(GetGameInstance())->DrainMana(ManaRequirement);
	}
}

void AGruppe7_FantasyGameCharacter::MagiAttack()
{	
	switch (SpellSelect)
	{
	case 0:
		AGruppe7_FantasyGameCharacter::MagiProjectile();
		break;
	case 1:
		AGruppe7_FantasyGameCharacter::MagiFireCone();
		break;
	case 2:
		AGruppe7_FantasyGameCharacter::MagiThornCircle();
		break;
	case 3:
		AGruppe7_FantasyGameCharacter::MagiHealing();
		break;
	}
}

void AGruppe7_FantasyGameCharacter::MagiAttackStop()
{
	SpellIsContinuous = false;
}

void AGruppe7_FantasyGameCharacter::ManaPotion(float ManaRestore)
{	
	Cast<UFantasyGameInstance>(GetGameInstance())->RestoreMana(ManaRestore);

	//Spiller av VFX.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ManaPickUpFX, GetTransform(), true);

	//Spiller av SFX.
	//UGameplayStatics::PlaySound2D(GetWorld(), ManaPickUpSound, 1.f, 1.f, 0.f);
}

void AGruppe7_FantasyGameCharacter::HealthPotion(float HealthRestore)
{	
	Cast<UFantasyGameInstance>(GetGameInstance())->RestoreHealth(HealthRestore);

	//Spiller av VFX.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealthPickUpFX, GetTransform(), true);

	//Spiller av SFX.
	//UGameplayStatics::PlaySound2D(GetWorld(), HealthPickUpSound, 1.f, 1.f, 0.f);
}

void AGruppe7_FantasyGameCharacter::PowerUp_Speed()
{	
	// Power-up som booster movement speed.

	// Timer system.
	GetWorldTimerManager().SetTimer(SpeedPowerUpTimerHandle, this, &AGruppe7_FantasyGameCharacter::PowerUp_SpeedOver, SpeedPowerUpDuration, false);

	// DEBUG.
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, TEXT("Power-up activated!"));

	// Prevents multiple power-ups at once.
	PlayerHasPowerup = true;

	PlayerMovementSpeed = 1200.f;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMovementSpeed;
}

void AGruppe7_FantasyGameCharacter::PowerUp_SpeedOver()
{
	// DEBUG.
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, TEXT("Power-up is over!"));

	PlayerHasPowerup = false;

	PlayerMovementSpeed = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMovementSpeed;

	GetWorld()->GetTimerManager().ClearTimer(SpeedPowerUpTimerHandle);
}

void AGruppe7_FantasyGameCharacter::Respawner()
{	
	switch (CurrentLevel)
	{
	default:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("SOMETHING WENT WRONG WITH THE RESPAWNING!"));
		break;
	case 1:
		GetWorld()->ServerTravel(FString("/Game/Maps/Level01_TheForest"));
		break;
	case 2:
		GetWorld()->ServerTravel(FString("/Game/Maps/Level02_TheCastle"));
		break;
	case 3:
		GetWorld()->ServerTravel(FString("/Game/Maps/Level03_BossRoom"));
		break;
	}

	Cast<UFantasyGameInstance>(GetGameInstance())->RestoreHealth(100.f);
	Cast<UFantasyGameInstance>(GetGameInstance())->RestoreHealth(100.f);
}

void AGruppe7_FantasyGameCharacter::PlayerDamageSound()
{
	//Random hit sound.
	int random = FMath::RandRange(0,1);

	switch (random)
	{
	default:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("SOMETHING WENT WRONG WITH SOUND! FUNCTION: PlayerDamageSound()"));
		break;
	case 0:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("Crash!"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageSound01, GetActorLocation());
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("Smack!"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageSound02, GetActorLocation());
		break;
	}

	//Random scream sound.
	RandomInt = FMath::RandRange(0, 1);

	switch (RandomInt)
	{
	default:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("SOMETHING WENT WRONG WITH SOUND! FUNCTION: PlayerDamageSound()"));
		break;
	case 0:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Orange, TEXT("Au!"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), PainSound01, GetActorLocation());
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Orange, TEXT("Ouch!"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), PainSound02, GetActorLocation());
		break;
	}
}

void AGruppe7_FantasyGameCharacter::PlayerAttackSound()
{
	//Random attack sound.
	RandomInt = FMath::RandRange(0, 1);

	switch (RandomInt)
	{
	default:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("SOMETHING WENT WRONG WITH SOUND! FUNCTION: PlayerAttackSound()"));
		break;
	case 0:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("SWOOSH!"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound01, GetActorLocation());
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("SWISH!"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound02, GetActorLocation());
		break;
	}

	//Random yell sound.
	int random = FMath::RandRange(0, 1);

	switch (random)
	{
	default:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("SOMETHING WENT WRONG WITH SOUND! FUNCTION: PlayerAttackSound()"));
		break;
	case 0:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Orange, TEXT("Huya!"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), YellSound01, GetActorLocation());
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Orange, TEXT("Ha!"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), YellSound02, GetActorLocation());
		break;
	}
}

void AGruppe7_FantasyGameCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{	
	////////////////////////////////////////////////////////////////////////////////
	// ENEMY ATTACKS.

	if (OtherActor->IsA(AEnemyAttackBox::StaticClass()))
	{
		OtherActor->Destroy();

		Cast<UFantasyGameInstance>(GetGameInstance())->DrainHealth(DamageFromEnemy);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetTransform(), true);

		PlayerDamageSound();

		if (Health <= 0.f)
		{	
			// DEBUG - Erstatt med en effekt?
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Purple, TEXT("YOU DIED! LOL n00b!"));

			Respawner();
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// POTIONS AND RESTORATIVE ITEMS.

	if (OtherActor->IsA(AWizardCloud::StaticClass()) && Mana < 1.f)
	{
		OtherActor->SetActorLocation(FVector(0.f, 0.f, -200.f));

		AGruppe7_FantasyGameCharacter::ManaPotion(1.f);
	}

	if (OtherActor->IsA(AManaPotion::StaticClass()) && Mana < 1.f)
	{
		OtherActor->Destroy();

		AGruppe7_FantasyGameCharacter::ManaPotion(0.25f);
	}

	if (OtherActor->IsA(AHealthPotion::StaticClass()) && Health < 1.f)
	{
		OtherActor->Destroy();
		
		AGruppe7_FantasyGameCharacter::HealthPotion(0.25f);
	}

	////////////////////////////////////////////////////////////////////////////////
	// POWER-UPS.

	if (OtherActor->IsA(APowerUp_Speed::StaticClass()) && (PlayerMovementSpeed == 600.f) && (PlayerHasPowerup == false))
	{	
		if (!PlayerHasPowerup)
		{
			OtherActor->Destroy();

			AGruppe7_FantasyGameCharacter::PowerUp_Speed();
		}
		else
		{	
			// DEBUG - Erstatt med en effekt?
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, TEXT("A Power-up is already active!"));
		}
	}
}