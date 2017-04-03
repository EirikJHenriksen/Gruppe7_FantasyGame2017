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
	//BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	//BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void AEnemy_AI_Controller::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);

	AEnemyBaseClass *Char = Cast<AEnemyBaseClass>(InPawn);

		//if (Char && Char->BotBehavior)
		//{
		//	BlackboardComp->InitializeBlackboard(*Char->BotBehavior->BlackboardAsset);

		//	EnemyKeyID = BlackboardComp->GetKeyID("Target");

		//	BehaviorComp->StartTree(*Char->BotBehavior);
		//}
}

// Called every frame
void AEnemy_AI_Controller::Tick(float DeltaTime)
{
Super::Tick(DeltaTime);

// Get distance
float Distance = Cast<AEnemyBaseClass>(GetCharacter())->GetDistanceToPlayer();


// If distance is less than ~300.f 
if (Distance < 700.f)
	{
		// runs to the player
		MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), 5.f, true, true, true, 0, true);

	} else if (Distance > 700.f)
	{
		// runs to the player

		// go home
		FVector MyHome = Cast<AEnemyBaseClass>(GetCharacter())->GetMyStartLocation();
		MoveToLocation(MyHome, 5.f, true, true, true, true, 0,true);
	}
}