// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "EnemyBaseClass.h"
#include "MagicProjectile.h"
#include "ConeOfFire.h"
#include "CircleOfThorns.h"
#include "PhysAttackBox.h"
#include "Gruppe7_FantasyGameCharacter.h"
#include "FantasyGameInstance.h"
#include "ManaPotion.h"
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

	// get Mana Potion Blueprint
	static ConstructorHelpers::FObjectFinder<UClass> SpawningManaFinder(TEXT("Blueprint'/Game/Blueprints/ManaPotion_BP.ManaPotion_BP'_C'"));
	SpawningMana = SpawningManaFinder.Object;
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


	//////////////I moved all this to CanSeePlayer
	//// Updates DistanceToPlayer
	//UpdateDistance();

	//// Enemy physical attack. - should move to controller ///////////
	//if (DistanceToPlayer < 100.f)
	//{
	//	++BadTimer;
	//	if (BadTimer > 60)
	//	{
	//		AEnemyBaseClass::MeleeAttack();
	//		BadTimer = 0;
	//	}
	//}
		//ClearSightCheck();
}

// getters
float AEnemyBaseClass::GetDistanceToPlayer() { return DistanceToPlayer; }
FVector AEnemyBaseClass::GetMyStartLocation() { return MyStartLocation; }

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

// gets the distance to player
void AEnemyBaseClass::UpdateDistance()
{
	CurrentPlayerLocation = Cast<UFantasyGameInstance>(GetGameInstance())->GetPlayerLocation();
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
	if (HealthPoints <= 0.f) // dies
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyDeathSound, GetActorLocation());


		// spawn a potion - maybe?
		GetWorld()->SpawnActor<AManaPotion>(SpawningMana, GetActorLocation(), GetActorRotation());


		Destroy();
	}
	else // is not dead, just gets hurt
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyHurtSound, GetActorLocation());
	}
}

// line trace to check if enemy sees player, or if line of sight is blocked
bool AEnemyBaseClass::CanSeePlayer()
{
	if (GetWorld()->GetFirstPlayerController()->GetCharacter())
	{
		// Updates DistanceToPlayer
		UpdateDistance();

		if (DistanceToPlayer < 900.f)
		{

			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Purple, TEXT("Player within distance"));
		
			FHitResult hitResult;
			FVector MyLocation = GetActorLocation();
			CurrentPlayerLocation = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorLocation();
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

				// Enemy physical attack. - should move to controller ///////////
				if (DistanceToPlayer < 100.f)
				{
					++BadTimer;
					if (BadTimer > 20)
					{
						AEnemyBaseClass::MeleeAttack();
						BadTimer = 0;
					}
				}
				return true;
			}


		}
		else
		{
			// player not close enough, so no point in running the other tests
			return false;
		}


	}
	else {
		// there is no player available, so more tests could crash the engine
		return false;
	}

		




	

}