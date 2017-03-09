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
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void AEnemy_AI_Controller::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);

	AEnemyBaseClass *Char = Cast<AEnemyBaseClass>(InPawn);

		if (Char && Char->BotBehavior)
		{
			BlackboardComp->InitializeBlackboard(*Char->BotBehavior->BlackboardAsset);

			EnemyKeyID = BlackboardComp->GetKeyID("Target");

			BehaviorComp->StartTree(*Char->BotBehavior);
		}
}
