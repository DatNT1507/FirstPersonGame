// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTT_FindRandomPatrol.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/EnemyKeys.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FindRandomPatrol::UBTT_FindRandomPatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Patrol");
}

EBTNodeResult::Type UBTT_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AI Controller and its Pawn
	AAIController* const AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* const AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	// Get Navigation System
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;

	// Find a random point within 1500 units of the AI's current location
	FNavLocation RandomLocation;
	FVector Origin = AIPawn->GetActorLocation();

	if (NavSystem->GetRandomReachablePointInRadius(Origin, 1500.0f, RandomLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(EnemyKeys::PatrolPos, RandomLocation.Location);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
