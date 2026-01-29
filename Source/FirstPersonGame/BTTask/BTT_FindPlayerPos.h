// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FindPlayerPos.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONGAME_API UBTT_FindPlayerPos : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_FindPlayerPos(FObjectInitializer const& ObjectInitializer);
protected:
	// Override ExecuteTask function
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
