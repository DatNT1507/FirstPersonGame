// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTT_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "../AI/EnemyKeys.h"
#include "AI/Enemy_Controller.h"
#include "Kismet/GameplayStatics.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AI Controller
	AAIController* const AIController = Cast<AAIController>(OwnerComp.GetAIOwner());

	// Get Navigation System
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (AIController && NavSystem)
	{
		FVector Location = AIController->GetBlackboardComponent()->GetValueAsVector(EnemyKeys::PlayerPos);

		AIController->MoveToLocation(Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
	return EBTNodeResult::Succeeded;
}
