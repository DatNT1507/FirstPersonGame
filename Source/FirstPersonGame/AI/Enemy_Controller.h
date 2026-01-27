// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy_Controller.generated.h"

/**
 * 
 */
class UBehaviorTreeComponent;
class UBehaviorTree;

UCLASS()
class FIRSTPERSONGAME_API AEnemy_Controller : public AAIController
{
	GENERATED_BODY()

public:
	AEnemy_Controller(FObjectInitializer const& ObjectInitializer);

	// Override
	virtual void OnPossess(APawn* InPawn) override;

	// Getter for Blackboard
	class UBlackboardComponent* GetBlackboard() const { return Blackboard; }

	// The Behavior Tree Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	// The Behavior Tree asset to use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;
};