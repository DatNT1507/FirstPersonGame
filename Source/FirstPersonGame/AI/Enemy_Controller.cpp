// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Enemy_Controller.h"
#include "AI/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AI/EnemyKeys.h"
#include "../Components/AdvancedPathFollowingComponent.h"

AEnemy_Controller::AEnemy_Controller(FObjectInitializer const& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UAdvancedPathFollowingComponent>(TEXT("PathFollowingComponent")))
{
	// Create the Behavior Tree Component
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    
	// Create the Blackboard Component
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	SightInitialization();
}

void AEnemy_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cast Enemy Pawn to access its properties
	if (AEnemy* PossessedEnemy = Cast<AEnemy>(InPawn))
	{
		// Get the custom PathFollowingComponent 
		if (UAdvancedPathFollowingComponent* CustomPathComp = Cast<UAdvancedPathFollowingComponent>(GetPathFollowingComponent()))
		{
			// Set the movement style based on the possessed enemy's desired movement style
			CustomPathComp->CurrentMovementStyle = PossessedEnemy->MovementStyle;
		}
	}
	
	if (BehaviourTree)
	{
		RunBehaviorTree(BehaviourTree);
	}
}

void AEnemy_Controller::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	// Check if the sensed actor is the player
	if (Actor->ActorHasTag("Player") || Actor->IsA(APawn::StaticClass()))
	{
		// Update Blackboard keys based on whether we see them or lost them
		GetBlackboard()->SetValueAsBool(EnemyKeys::CanSeePlayer, Stimulus.WasSuccessfullySensed());
	}
}

void AEnemy_Controller::SightInitialization()
{
	// Create Perception Component
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	// Create and Configure Sight
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 2500.f;
	SightConfig->PeripheralVisionAngleDegrees = 45.f;
    
	// Detection by affiliation: Ensure Neutrals is true for the Player
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// Bind the event
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy_Controller::OnTargetDetected);
}
