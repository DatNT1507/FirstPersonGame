// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Jump_Enemy.generated.h"

UCLASS()
class FIRSTPERSONGAME_API AJump_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJump_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// The handle that tracks our 2-second timer
	FTimerHandle JumpTimerHandle;

	// The function that the timer will call
	void PerformJump();

	// How often the enemy should jump (in seconds)
	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpInterval = 2.0f;
};
