// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Gruppe7_FantasyGame.h"
#include "MagicProjectile.h"
#include "ConeOfFire.h"
#include "CircleOfFire.h"
#include "PhysAttackBox.h"
#include "ManaPotion.h"
#include "HealthPotion.h"
#include "PowerUp_Speed.h"
#include "FantasyGameInstance.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
//DEBUG.
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
//DEBUG.
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
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Textures/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	/////////////////////////////////////////////////////
	// Synchronizes variables with GameInstanceVariables.
	//Cast<UFantasyGameInstance>(GetGameInstance())->GetHealth());
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGruppe7_FantasyGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Magic and physical attacks.
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::PhysAttack);
	PlayerInputComponent->BindAction("CastSpell", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::MagiAttack);
	PlayerInputComponent->BindAction("SpellSwapUp", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::SpellSwapUp);
	PlayerInputComponent->BindAction("SpellSwapDown", IE_Pressed, this, &AGruppe7_FantasyGameCharacter::SpellSwapDown);

	// WASD - Movement.
	PlayerInputComponent->BindAxis("MoveForward", this, &AGruppe7_FantasyGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGruppe7_FantasyGameCharacter::MoveRight);

	// Jumping.
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGruppe7_FantasyGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGruppe7_FantasyGameCharacter::TouchStopped);
}

void AGruppe7_FantasyGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Synchronizes Player variables with game instance.
	Health = Cast<UFantasyGameInstance>(GetGameInstance())->GetHealth();
	Mana = Cast<UFantasyGameInstance>(GetGameInstance())->GetMana();


	FHitResult Hit;
	bool HitResult = false;

	HitResult = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_WorldStatic), true, Hit);

	if (HitResult)
	{
		FVector CursorFV = Hit.ImpactNormal;
		FRotator CursorR = CursorFV.Rotation();
		CursorToWorld->SetWorldLocation(Hit.Location);
		CursorToWorld->SetWorldRotation(CursorR);

		FVector CursorLocation = Hit.Location;
		UE_LOG(LogTemp, Warning, TEXT("Cursor location %s!"), *CursorLocation.ToString());

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
		++SpellSelect;
	}
	if (SwapUp == false)
	{
		--SpellSelect;
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
		SpellSelect = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Magic Projectile Selected"));
		break;
	case -1:
		SpellSelect = 3;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Circle of Fire Selected"));
		break;
	}
}

void AGruppe7_FantasyGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AGruppe7_FantasyGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
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

	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = GetActorLocation();
		FVector Offset = FVector(0.0f, 0.0f, 0.0f);

		FRotator ProjectileRotation = GetActorRotation();

		Location += Offset;

		GetWorld()->SpawnActor<APhysAttackBox>(PhysAttackBlueprint, GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation());

		//Spiller lyd.
		//UGameplayStatics::PlaySound2D(GetWorld(), AttackSound, 1.f, 1.f, 0.f);
	}
}

void AGruppe7_FantasyGameCharacter::MagiProjectile()
{
	//Set the required mana for casting this spell.
	float ManaRequirement{ 0.01f };

	UWorld* World = GetWorld();
	if (World && (Mana >= ManaRequirement))
	{
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

void AGruppe7_FantasyGameCharacter::MagiFireCone()
{
	//Set the required mana for casting this spell.
	float ManaRequirement{ 0.01f };

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

		Mana -= ManaRequirement;
	}
}

void AGruppe7_FantasyGameCharacter::MagiFireCircle()
{
	//Set the required mana for casting this spell.
	float ManaRequirement{ 0.01f };

	UWorld* World = GetWorld();
	if (World && (Mana >= ManaRequirement))
	{	
		FVector Location = GetActorLocation();
		FVector Offset = FVector(0.0f, 0.0f, 0.0f);

		FRotator ProjectileRotation = GetActorRotation();

		Location += Offset;

		GetWorld()->SpawnActor<ACircleOfFire>(MagicFireCircleBlueprint, GetActorLocation() + GetActorForwardVector() * 1.f, GetActorRotation());

		//Spiller skytelyd.
		//UGameplayStatics::PlaySound2D(GetWorld(), CastSound, 1.f, 1.f, 0.f);

		Mana -= ManaRequirement;
	}
}

void AGruppe7_FantasyGameCharacter::MagiHealing()
{	
	// DEBUG.
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("ATTEMPT AT HEALING!!!"));

	if (Health != 1.f)
	{
		//Set the required mana for casting this spell.
		float ManaRequirement{ 0.01f };
		float HealthRestoration{ 0.1f };

		// DEBUG.
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("HEALING!!!"));

		Health += HealthRestoration;

		if (Health > 1.f)
		{
			Health = 1.f;
		}

		Mana -= ManaRequirement;
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
		AGruppe7_FantasyGameCharacter::MagiFireCircle();
		break;
	case 3:
		AGruppe7_FantasyGameCharacter::MagiHealing();
		break;
	}
}

void AGruppe7_FantasyGameCharacter::ManaPotion()
{	
	// Mengde mana som regenereres av potion.
	float ManaRestore{ 0.25f };

	Cast<UFantasyGameInstance>(GetGameInstance())->RestoreMana(ManaRestore);

	//Spiller av VFX.
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ManaPickUpFX, GetTransform(), true);

	//Spiller av SFX.
	//UGameplayStatics::PlaySound2D(GetWorld(), ManaPickUpSound, 1.f, 1.f, 0.f);
}

void AGruppe7_FantasyGameCharacter::HealthPotion()
{	
	// Mengde health som regenereres av potion.
	float HealthRestore{ 0.25f };

	Cast<UFantasyGameInstance>(GetGameInstance())->RestoreHealth(HealthRestore);

	//Spiller av VFX.
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealthPickUpFX, GetTransform(), true);

	//Spiller av SFX.
	//UGameplayStatics::PlaySound2D(GetWorld(), HealthPickUpSound, 1.f, 1.f, 0.f);
}

void AGruppe7_FantasyGameCharacter::PowerUp_Speed()
{	
	// Power-up som booster movement speed.

	// DEBUG.
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, TEXT("Power-up activated!"));

	// Prevents multiple power-ups at once.
	PlayerHasPowerup = true;

	PlayerMovementSpeed = 1200.f;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMovementSpeed;

	// F� timer til � fungere.

	if (!PlayerHasPowerup)
	{
		PlayerHasPowerup = true;

		GetWorld()->GetTimerManager().SetTimer(PowerUpTimerHandle, this, &AGruppe7_FantasyGameCharacter::PowerUp_SpeedOver, MaxPowerTime, false);
	}
}

void AGruppe7_FantasyGameCharacter::PowerUp_SpeedOver()
{
	// DEBUG.
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, TEXT("Power-up is over!"));

	PlayerHasPowerup = false;

	PlayerMovementSpeed = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMovementSpeed;

	GetWorld()->GetTimerManager().ClearTimer(PowerUpTimerHandle);
}

void AGruppe7_FantasyGameCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AManaPotion::StaticClass()) && Mana < 1.f)
	{
		OtherActor->Destroy();

		AGruppe7_FantasyGameCharacter::ManaPotion();
	}

	if (OtherActor->IsA(AHealthPotion::StaticClass()) && Health < 1.f)
	{
		OtherActor->Destroy();
		
		AGruppe7_FantasyGameCharacter::HealthPotion();
	}

	if (OtherActor->IsA(APowerUp_Speed::StaticClass()) && (PlayerMovementSpeed == 600.f) && (PlayerHasPowerup == false))
	{
		OtherActor->Destroy();

		// Flytt kode inn i PowerUp_Speed klassen!
		//Cast<APowerUp_Speed>(OtherActor)->PlayerPickUp();

		AGruppe7_FantasyGameCharacter::PowerUp_Speed();
	}
}