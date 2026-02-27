// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Mover.h"

#include "AITypes.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetOwner()->GetActorLocation();
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoverTick(DeltaTime);
}

void UMover::MoverTick(float DeltaTime)
{
	if (CanMove)
	{
		TargetLocation = StartLocation + MoveOffset;
	}
	else
	{
		TargetLocation = StartLocation;
	}
	
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	
	IsReachTarget = CurrentLocation.Equals(TargetLocation);
	
	if (!IsReachTarget)
	{
		float MoveSpeed = MoveOffset.Size() / MoveTime;
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
		GetOwner()->SetActorLocation(NewLocation);
	}
}

