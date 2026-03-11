// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FirstPersonGameGameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class AFirstPersonGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFirstPersonGameGameMode();

protected:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	float ElapsedTime = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "GameMode")
	bool bIstimerRunning = false;

	UFUNCTION()
	void StopTimer();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Logic")
	void OnGameEndedUI(bool bIsWin);

public:
	// Function to check player win or lose
	UFUNCTION(BlueprintCallable, Category = "Game Logic")
	void HandleGameEnd(bool bIsWin);
	
};



