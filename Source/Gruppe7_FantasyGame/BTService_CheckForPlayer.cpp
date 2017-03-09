// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Gruppe7_FantasyGameCharacter.h"
#include "EnemyBaseClass.h"
#include "Enemy_AI_Controller.h"
#include "BTService_CheckForPlayer.h"


UBTService_CheckForPlayer::UBTService_CheckForPlayer()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemy_AI_Controller *EnemyPC = Cast<AEnemy_AI_Controller>(OwnerComp.GetAIOwner());

	if (EnemyPC)
	{
		AGruppe7_FantasyGameCharacter *Enemy = Cast<AGruppe7_FantasyGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (Enemy)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyPC->EnemyKeyID, Enemy);
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Enemy is here!");
		}
	}
}
