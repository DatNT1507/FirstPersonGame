// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zigzag_Enemy.generated.h"

UCLASS()
class FIRSTPERSONGAME_API AZigzag_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZigzag_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Tracks time to feed into the Sine math function
	float RunningTime;

	// How fast the enemy switches from left to right
	UPROPERTY(EditAnywhere, Category = "Movement|Zigzag")
	float ZigzagFrequency = 3.0f;

	// How hard the enemy pushes to the side (0.0 to 1.0 is standard input scale)
	UPROPERTY(EditAnywhere, Category = "Movement|Zigzag")
	float ZigzagIntensity = 1.0f;

	void ZigzagMovement(float DeltaTime);
};
