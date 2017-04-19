// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "EnemyBaseClass.h"
#include "MagicProjectile.h"
#include "ConeOfFire.h"
#include "CircleOfThorns.h"
#include "PhysAttackBox.h"
#include "KnockbackSphere.h"
#include "Gruppe7_FantasyGameCharacter.h"
#include "FantasyGameInstance.h"
#include "EnemyAttackBox.h"

// Sets default values
AEnemyBaseClass::AEnemyBaseClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->bGenerateOverlapEvents = true;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBaseClass::OnOverlap);

	// Sets control parameters
	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->AirControlBoostMultiplier = 0.f;
	GetCharacterMovement()->AirControlBoostVelocityThreshold = 0.f;
}

// Called when the game starts or when spawned
void AEnemyBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
	MyStartLocation = GetActorLocation();
}

// Called every frame
void AEnemyBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Keeps track of the players location. IMPORTANT THAT IT STAYS IN TICK FUNCTION.
	CurrentPlayerLocation = Cast<UFantasyGameInstance>(GetGameInstance())->GetPlayerLocation();

	// Updates DistanceToPlayer
	UpdateDistance();

	// Enemy physical attack. - should move to controller ///////////
	if (DistanceToPlayer < 100.f)
	{
		AttackTimer += 1.f;
		if (AttackTimer > 30.f)
		{
			AEnemyBaseClass::MeleeAttack();
			AttackTimer = 0.f;
		}
	}
		//ClearSightCheck();
}

// the enemies melee attack
void AEnemyBaseClass::MeleeAttack()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = GetActorLocation();
		FVector Offset = FVector(0.0f, 0.0f, 0.0f);

		FRotator ProjectileRotation = GetActorRotation();

		Location += Offset;

		GetWorld()->SpawnActor<AEnemyAttackBox>(EnemyAttackBlueprint, GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation());

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyMeleeAttackSound, GetActorLocation());
	}
}

// getters
float AEnemyBaseClass::GetDistanceToPlayer() {	return DistanceToPlayer; }
FVector AEnemyBaseClass::GetMyStartLocation() {	return MyStartLocation; }

// gets the distance to player
void AEnemyBaseClass::UpdateDistance()
{
	DistanceVector = GetActorLocation() - CurrentPlayerLocation;

	DistanceToPlayer = DistanceVector.Size();
}

// When attacked by player
void AEnemyBaseClass::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Physic attack
	if (OtherActor->IsA(APhysAttackBox::StaticClass()))
	{
		OtherActor->Destroy();
		HealthPoints -= DamageMelee;
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetTransform(), true);

		// Pushes the enemy back. They get slowed down for some time. (FORCE, DURATION).
		EnemyIsHit(1000.f, 1.5f);
	}

	if (OtherActor->IsA(AKnockbackSphere::StaticClass()))
	{
		// Pushes the enemy back. They get slowed down for some time. (FORCE, DURATION).
		EnemyIsHit(500.f, 1.f);
	}

	// Magic Projectile - WATER
	if (OtherActor->IsA(AMagicProjectile::StaticClass()))
	{
		OtherActor->Destroy();

		switch (Element)
		{
		case ElementsEnum::FIRE:
			// weak
			HealthPoints -= DamageMyWeakness;
			break;
		case ElementsEnum::WATER:
			// ok
			HealthPoints -= DamageMyEqual;
			break;
		case ElementsEnum::NATURE:
			// heal?
			HealthPoints -= DamageLeastEffective;
			break;
		default:
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("error - EnemyBaseClass - attacked"));
		}
	}

	// Circle of thorns - NATURE
	if (OtherActor->IsA(ACircleOfThorns::StaticClass()))
	{
		switch (Element)
		{
		case ElementsEnum::WATER:
			// weak
			HealthPoints -= DamageMyWeakness;
			break;
		case ElementsEnum::NATURE:
			// ok
			HealthPoints -= DamageMyEqual;
			break;
		case ElementsEnum::FIRE:
			// heal?
			HealthPoints -= DamageLeastEffective;
			break;
		default:
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("error - EnemyBaseClass - attacked"));
		}
	}

	// Cone of fire - FIRE
	if (OtherActor->IsA(AConeOfFire::StaticClass()))
	{
		switch (Element)
		{
		case ElementsEnum::NATURE:
			// weak
			HealthPoints -= DamageMyWeakness;
			break;
		case ElementsEnum::FIRE:
			// ok
			HealthPoints -= DamageMyEqual;
			break;
		case ElementsEnum::WATER:
			// heal?
			HealthPoints -= DamageLeastEffective;
			break;
		default:
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("error - EnemyBaseClass - attacked"));
		}
	}
	DeathCheck();
}

// Check if dead
void AEnemyBaseClass::DeathCheck()
{
	if (HealthPoints <= 0.f)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyDeathSound, GetActorLocation());
		Destroy();
	}
	else {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyHurtSound, GetActorLocation());
	}
}

void AEnemyBaseClass::EnemyIsHit(float force, float duration)
{
	// Pushes back the enemy.
	//FVector PlayerLocation = Cast<UFantasyGameInstance>(GetGameInstance())->GetPlayerLocation();

	FVector KnockbackVector = GetActorLocation() - CurrentPlayerLocation;
	KnockbackVector.Normalize();

	LaunchCharacter((KnockbackVector * force) + FVector(0.f, 0.f, 20.f), false, false);

	GetCharacterMovement()->MaxWalkSpeed = 100.f;

	GetWorldTimerManager().SetTimer(SlowdownTimerHandle, this, &AEnemyBaseClass::SlowdownOver, duration, false);
}

void AEnemyBaseClass::SlowdownOver()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	GetWorld()->GetTimerManager().ClearTimer(SlowdownTimerHandle);
}

// line trace to check if enemy sees player, or if line of sight is blocked
bool AEnemyBaseClass::ClearSightCheck()
{
	if (GetWorld()->GetFirstPlayerController()->GetCharacter())
	{
	FHitResult hitResult;
	FVector MyLocation = GetActorLocation();
	FCollisionQueryParams collisionParams = FCollisionQueryParams();
	collisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(hitResult, MyLocation, CurrentPlayerLocation, ECC_Visibility, collisionParams); ////////

	if (hitResult.bBlockingHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("view blocked"));
		return false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("sees player"));
		return true;
	}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("No player"));
		return false;
	}

}