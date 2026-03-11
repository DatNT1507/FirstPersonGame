// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonGameGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

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

void AFirstPersonGameGameMode::HandleGameEnd(bool bIsWin)
{
	StopTimer();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		// Freeze the world
		PC->SetPause(true);

		// Show mouse
		PC->bShowMouseCursor = true;

		// Restrict input
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}

	// Trigger the Blueprint Event to spawn the visual UI!
	OnGameEndedUI(bIsWin);
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
