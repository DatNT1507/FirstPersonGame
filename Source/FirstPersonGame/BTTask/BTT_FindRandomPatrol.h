// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FindRandomPatrol.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONGAME_API UBTT_FindRandomPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTT_FindRandomPatrol(FObjectInitializer const& ObjectInitializer);
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
