// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "FinalBoss.h"
#include "MagicProjectile.h"
#include "ConeOfFire.h"
#include "CircleOfThorns.h"
#include "PhysAttackBox.h"
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

	fightInProgress = true; // Synkroniser med BossFightActive variablen fra GameInstance.

	canTeleport = true;
}

// Called every frame
void AFinalBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Keeps track of the players location. IMPORTANT THAT IT STAYS IN TICK FUNCTION.
	CurrentPlayerLocation = Cast<UFantasyGameInstance>(GetGameInstance())->GetPlayerLocation();

	// Updates DistanceToPlayer
	UpdateDirection();

	// Turns boss towards player.
	SetActorRotation(LookVector.Rotation() + FRotator(0.f, -180.f, 0.f));
	


	if (fightInProgress && canTeleport)
	{	
		canTeleport = false;

		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, TEXT("TIMER - ACTIVE"));

		RandomTeleportTime = FMath::RandRange(RandomMin, RandomMax);

		GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &AFinalBoss::Teleport, RandomTeleportTime, false);

		//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORTER - FUNCTION OVER"));
	}

	//if (DistanceToPlayer < 100.f)
	//{
	//	AttackTimer += 1.f;
	//	if (AttackTimer > 30.f)
	//	{
	//		Attack();
	//		AttackTimer = 0.f;
	//	}
	//}
}

// gets the distance to player
void AFinalBoss::UpdateDirection()
{
	LookVector = GetActorLocation() - CurrentPlayerLocation;
	//DistanceToPlayer = DistanceVector.Size();
}

// Called to bind functionality to input
void AFinalBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFinalBoss::Teleport()
{	
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORTER - FUNCTION IN PROGRESS"));

	// Teleports the boss to one of three random spots.
	int random = FMath::RandRange(0, 2);

	// FIKS SÅ HAN IKKE TELEPORTERER TIL SAMME STED SOM HAN STÅR!

	switch (random)
	{
	default:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("SOMETHING WENT WRONG WITH BOSS TELEPORT! FUNCTION: Teleport()"));
		break;
	case 0:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORT - FIRE"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX01, GetActorLocation());
		//VFX.
		SetActorLocation(FVector(FireX, FireY, FireZ), false);
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORT - WATER"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX02, GetActorLocation());
		//VFX.
		SetActorLocation(FVector(WaterX, WaterY, WaterZ), false);
		break;
	case 2:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("TELEPORT - NATURE"));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX03, GetActorLocation());
		//VFX.
		SetActorLocation(FVector(NatureX, NatureY, NatureZ), false);
		break;
	}

	GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);

	canTeleport = true;
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

		if (!fightInProgress)
		{
			//Spill av partikkeleffekt og lyd. - Onskapsfull latter som viser at angrepet ikke gjør noe.
		}

	}

	// Circle of thorns - NATURE
	if (OtherActor->IsA(ACircleOfThorns::StaticClass()))
	{

	}

	// Cone of fire - FIRE
	if (OtherActor->IsA(AConeOfFire::StaticClass()))
	{

	}
}