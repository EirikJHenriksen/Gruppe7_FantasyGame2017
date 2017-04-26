// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "EnemyBaseClass.h"
#include "Enemy_AI_Controller.h"


AEnemy_AI_Controller::AEnemy_AI_Controller()
{
	// all enemies start out in idle state
	State = StateEnum::IDLE;
}

void AEnemy_AI_Controller::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);
	AEnemyBaseClass *Char = Cast<AEnemyBaseClass>(InPawn);
}
//
// Called every frame
void AEnemy_AI_Controller::Tick(float DeltaTime)
{
Super::Tick(DeltaTime);

// Each tick run the function fitting current state
switch (State)
{
case StateEnum::IDLE:
	// weak
	IdleState();
	break;
case StateEnum::FOLLOW:
	ApproachState();
	break;
case StateEnum::RETURN:
	ReturnState();
	break;
//case StateEnum::ATTACK:
//	AttackState();
//	break;
default:
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Switch not working in AIController"));
}

}

// FUNCTIONS FOR STATES START HERE
// IDLE
void AEnemy_AI_Controller::IdleState()
{
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Purple, TEXT("I am being idle"));

	if (Cast<AEnemyBaseClass>(GetCharacter())->CanSeePlayer())
	{
		State = StateEnum::FOLLOW;
	}

	//float Distance = Cast<AEnemyBaseClass>(GetCharacter())->GetDistanceToPlayer();
	//// if close enough, check if can see
	//if (Distance < 900.f)
	//	{
	//		
	//		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Purple, TEXT("Player within distance"));
	//		State = StateEnum::FOLLOW;
	//		
	//	}
}

// FOLLOW
void AEnemy_AI_Controller::ApproachState()
{
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Purple, TEXT("Following player"));

	// runs to the player
	MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), 5.f, true, true, true, 0, true);
	
	if (!Cast<AEnemyBaseClass>(GetCharacter())->CanSeePlayer())
	{
		State = StateEnum::RETURN;
		//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Purple, TEXT("GONE~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
	}

}

// RETURN
void AEnemy_AI_Controller::ReturnState()
{
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Purple, TEXT("Going home"));

	// go home
	FVector MyHome = Cast<AEnemyBaseClass>(GetCharacter())->GetMyStartLocation();
	MoveToLocation(MyHome, 5.f, true, true, true, true, 0, true);

	// when home location is close enough become idle
	if (true)
	{
		State = StateEnum::IDLE;
	}

}


/*
 Get distance
float Distance = Cast<AEnemyBaseClass>(GetCharacter())->GetDistanceToPlayer();

 If distance is less than ~300.f 
if (Distance < 700.f)
{
	 runs to the player

	MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), 5.f, true, true, true, 0, true);

}
else
{
	 runs to the player

	 go home
	FVector MyHome = Cast<AEnemyBaseClass>(GetCharacter())->GetMyStartLocation();
	MoveToLocation(MyHome, 5.f, true, true, true, true, 0, true);
}
*/