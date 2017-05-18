// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "FinalBoss.h"
#include "MagicProjectile.h"
#include "ConeOfFire.h"
#include "CircleOfThorns.h"
#include "PhysAttackBox.h"
#include "BossSpellFire.h"
#include "EnemyBaseClass.h"
#include "Gruppe7_FantasyGameCharacter.h"
#include "FantasyGameInstance.h"


// Sets default values
AFinalBoss::AFinalBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->bGenerateOverlapEvents = true;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFinalBoss::OnOverlap);
}

// Called when the game starts or when spawned
void AFinalBoss::BeginPlay()
{
	Super::BeginPlay();

	Health = 1.f;

	canBeHurt = false;

	canTeleport = true;

	isAttacking = false;

	firstTeleport = true;
}

// Called every frame
void AFinalBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Gets variable from gameInstance class.
	fightInProgress = Cast<UFantasyGameInstance>(GetGameInstance())->GetBossFightActive();

	// Keeps track of the players location. IMPORTANT THAT IT STAYS IN TICK FUNCTION.
	CurrentPlayerLocation = Cast<UFantasyGameInstance>(GetGameInstance())->GetPlayerLocation();

	// Updates DistanceToPlayer
	UpdateDirection();

	// Turns boss towards player.
	if (!IsDying || !IsDead)
	{
		SetActorRotation(LookVector.Rotation() + FRotator(0.f, 180.f, 0.f));
	}
	
	if (fightInProgress)
	{	
		if (firstTeleport)
		{	
			// PUT IN A SHORT DELAY.
			GetWorldTimerManager().SetTimer(FirstTeleportTimerHandle, this, &AFinalBoss::Teleport, 5.f, false);
			firstTeleport = false;
		}

		if (canTeleport && !isAttacking && !firstTeleport)
		{
			canTeleport = false;

			//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, TEXT("TIMER - ACTIVE"));

			RandomTeleportTime = FMath::RandRange(RandomMin, RandomMax);

			GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &AFinalBoss::Teleport, RandomTeleportTime, false);

			//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORTER - FUNCTION OVER"));
		}

		if (!canTeleport && !isAttacking && !firstTeleport && fightInProgress)
		{
			isAttacking = true;

			//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("ATTACK - ACTIVE"));

			RandomAttackTime = FMath::RandRange(AttackRandomMin, AttackRandomMax);

			GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AFinalBoss::Attack, RandomAttackTime, false);
		}
	}
}

// gets the distance to player
void AFinalBoss::UpdateDirection()
{
	LookVector = GetActorLocation() - CurrentPlayerLocation;
}

// Called to bind functionality to input
void AFinalBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFinalBoss::Teleport()
{	
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORTER - FUNCTION IN PROGRESS"));

	if (!canBeHurt)
	{
		canBeHurt = true;

		GetWorld()->GetTimerManager().ClearTimer(FirstTeleportTimerHandle);
	}

	// Teleports the boss to one of three random spots.
	int random = FMath::RandRange(0, 2);

	// FIKS s� dette fungerer bedre!

	switch (random)
	{
	default:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("SOMETHING WENT WRONG WITH BOSS TELEPORT! FUNCTION: Teleport()"));
		break;
	case 0:
		//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORT - FIRE"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX01, GetActorLocation());
		//VFX.
		if (Element != 0)
		{
			Element = 0;
			SetActorLocation(FVector(FireX, FireY, FireZ), false);
		}
		else if (Element != 1)
		{
			Element = 1;
			SetActorLocation(FVector(WaterX, WaterY, WaterZ), false);
		}
		else
		{
			Element = 2;
			SetActorLocation(FVector(NatureX, NatureY, NatureZ), false);
		}

		SummonEnemy();
		break;
	case 1:
		//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORT - WATER"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX02, GetActorLocation());
		//VFX.
		if (Element != 1)
		{
			Element = 1;
			SetActorLocation(FVector(WaterX, WaterY, WaterZ), false);
		}
		else if (Element != 2)
		{
			Element = 2;
			SetActorLocation(FVector(NatureX, NatureY, NatureZ), false);
		}
		else
		{
			Element = 0;
			SetActorLocation(FVector(FireX, FireY, FireZ), false);
		}

		SummonEnemy();
		break;
	case 2:
		//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORT - NATURE"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX03, GetActorLocation());
		//VFX.
		if (Element != 1)
		{
			Element = 2;
			SetActorLocation(FVector(NatureX, NatureY, NatureZ), false);
		}
		else if (Element != 0)
		{
			Element = 0;
			SetActorLocation(FVector(FireX, FireY, FireZ), false);
		}
		else
		{
			Element = 1;
			SetActorLocation(FVector(WaterX, WaterY, WaterZ), false);
		}

		SummonEnemy();
		break;
	}

	GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);

	canTeleport = true;
}

void AFinalBoss::Attack()
{
	UWorld* World = GetWorld();
	if (World)
	{
		GetWorld()->SpawnActor<ABossSpellFire>(SpellFireBlueprint, GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation());
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireAttackSound, GetActorLocation());
	}


	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	isAttacking = false;
}

void AFinalBoss::SummonEnemy()
{
	// Summons random enemy.
	int random = FMath::RandRange(0, 2);

	switch (random)
	{
	default:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("SOMETHING WENT WRONG WITH SUMMONS! FUNCTION: SummonEnemy()"));
		break;
	case 0:
		GetWorld()->SpawnActor<AEnemyBaseClass>(FireEnemyBlueprint, FVector(SpawnX, SpawnY, SpawnZ), GetActorRotation());
		break;
	case 1:
		GetWorld()->SpawnActor<AEnemyBaseClass>(WaterEnemyBlueprint, FVector(SpawnX, SpawnY, SpawnZ), GetActorRotation());
		break;
	case 2:
		GetWorld()->SpawnActor<AEnemyBaseClass>(NatureEnemyBlueprint, FVector(SpawnX, SpawnY, SpawnZ), GetActorRotation());
		break;
	}
}

void AFinalBoss::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Physic attack
	if (OtherActor->IsA(APhysAttackBox::StaticClass()))
	{
		OtherActor->Destroy();

	}

	// Magic Projectile - WATER
	if (OtherActor->IsA(AMagicProjectile::StaticClass()))
	{	
		OtherActor->Destroy();

		if (canBeHurt && Element == 0)
		{	
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("Tar damage!"));
			Health -= 0.05f;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Tar ikke damage!"));
			//Spill av partikkeleffekt og lyd. - Onskapsfull latter som viser at angrepet ikke gj�r noe. - LAG EGEN FUNKSJON!!!
		}
	}

	// Circle of thorns - NATURE
	if (OtherActor->IsA(ACircleOfThorns::StaticClass()))
	{
		if (canBeHurt && Element == 1)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("Tar damage!"));
			Health -= 0.05f;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Tar ikke damage!"));
			//Spill av partikkeleffekt og lyd. - Onskapsfull latter som viser at angrepet ikke gj�r noe. - LAG EGEN FUNKSJON!!!
		}
	}

	// Cone of fire - FIRE
	if (OtherActor->IsA(AConeOfFire::StaticClass()))
	{
		if (canBeHurt && Element == 2)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("Tar damage!"));
			Health -= 0.05f;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Tar ikke damage!"));
			//Spill av partikkeleffekt og lyd. - Onskapsfull latter som viser at angrepet ikke gj�r noe. - LAG EGEN FUNKSJON!!!
		}
	}

	DeathCheck();
}

void AFinalBoss::DeathCheck()
{
	if (Health <= 0.f)
	{	
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Night-Night d�de!"));

		// Erstatt med animasjon.
		IsDead = true;

		// Lets the game know that the game is won. 
		Cast<UFantasyGameInstance>(GetGameInstance())->SetGameIsWon(true);
	}
}