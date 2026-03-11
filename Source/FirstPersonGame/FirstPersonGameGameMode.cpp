// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonGameGameMode.h"

AFirstPersonGameGameMode::AFirstPersonGameGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ElapsedTime = 0.0f;
	bIstimerRunning = true;
}

void AFirstPersonGameGameMode::StopTimer()
{
	bIstimerRunning = false;
}

void AFirstPersonGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIstimerRunning)
	{
		ElapsedTime += DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("%f"), ElapsedTime);
	}
}
