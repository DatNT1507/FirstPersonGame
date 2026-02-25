// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AdvancedPathFollowingComponent.h"
#include "GameFramework/Pawn.h"
#include "../AI/Enemy_Controller.h"

UAdvancedPathFollowingComponent::UAdvancedPathFollowingComponent()
{
	RunningTime = 0.0f;
}

void UAdvancedPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	// Handle the forward movement along the NavMesh
	Super::FollowPathSegment(DeltaTime);

	// If it is set to Normal, stop here. No extra math needed!
	if (CurrentMovementStyle == EMovementStyle::Normal)
	{
		return; 
	}

	// Get the Pawn
	AAIController* AIController = Cast<AAIController>(GetOwner());
	if (!AIController) return;
	
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	// Calculate vectors for our math
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	FVector CurrentTarget = GetCurrentTargetLocation();
	FVector PathDirection = (CurrentTarget - CurrentLocation).GetSafeNormal();
	FVector PerpendicularDir = FVector::CrossProduct(FVector::UpVector, PathDirection).GetSafeNormal();

	// 5. Apply Zigzag Math if the Enum is set to Zigzag
	if (CurrentMovementStyle == EMovementStyle::Zigzag)
	{
		float SineValue = ZigzagMovement(DeltaTime);
		ControlledPawn->AddMovementInput(PerpendicularDir, SineValue * ZigzagIntensity);
	}
}

float UAdvancedPathFollowingComponent::ZigzagMovement(float DeltaTime)
{
	RunningTime += DeltaTime;

	return FMath::Sin(RunningTime * ZigzagFrequency);
}

