// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "EnemyBaseClass.h"
#include "MagicProjectile.h"
#include "ConeOfFire.h"
#include "CircleOfFire.h"
#include "PhysAttackBox.h"


// Sets default values
AEnemyBaseClass::AEnemyBaseClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->bGenerateOverlapEvents = true;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBaseClass::OnOverlap);

}

// Called when the game starts or when spawned
void AEnemyBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBaseClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBaseClass::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Physic attack
	if (OtherActor->IsA(APhysAttackBox::StaticClass()))
	{
		OtherActor->Destroy();
		HealthPoints -= 40.f;
	}

	// Magic Projectile
	if (OtherActor->IsA(AMagicProjectile::StaticClass()))
	{
		OtherActor->Destroy();
		HealthPoints -= 50.f;
	}

	// Circle of fine
	if (OtherActor->IsA(ACircleOfFire::StaticClass()))
	{
		OtherActor->Destroy();
		HealthPoints -= 40.f;
	}

	// Cone of fire
	if (OtherActor->IsA(AConeOfFire::StaticClass()))
	{
		OtherActor->Destroy();
		HealthPoints -= 40.f;
	}

	DeathCheck();
}

void AEnemyBaseClass::DeathCheck()
{
	//check if dead
	if (HealthPoints <= 0.f)
	{
		Destroy();
	}
}