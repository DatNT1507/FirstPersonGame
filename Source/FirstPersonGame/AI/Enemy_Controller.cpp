// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Enemy_Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemy_Controller::AEnemy_Controller(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	// Create the Behavior Tree Component
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    
	// Create the Blackboard Component
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AEnemy_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviourTree)
	{
		RunBehaviorTree(BehaviourTree);
	}
}