// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTT_FindPlayerPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "../AI/EnemyKeys.h"
#include "AI/Enemy_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBTT_FindPlayerPos::UBTT_FindPlayerPos(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Find Player Position");
}

EBTNodeResult::Type UBTT_FindPlayerPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AI Controller
	AEnemy_Controller* const AIController = Cast<AEnemy_Controller>(OwnerComp.GetAIOwner());

	// Get Navigation System
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	// Get Player Character
	APawn* const PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (NavSystem && PlayerPawn)
	{
		// Set Player Position in Blackboard
		AIController->GetBlackboardComponent()->SetValueAsVector(EnemyKeys::PlayerPos, PlayerPawn->GetActorLocation());
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
	return EBTNodeResult::Succeeded;
}
