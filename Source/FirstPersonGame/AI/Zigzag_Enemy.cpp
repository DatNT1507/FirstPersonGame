// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Zigzag_Enemy.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AZigzag_Enemy::AZigzag_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RunningTime = 0.0f;
}

// Called when the game starts or when spawned
void AZigzag_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZigzag_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZigzagMovement(DeltaTime);
}

// Called to bind functionality to input
void AZigzag_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZigzag_Enemy::ZigzagMovement(float DeltaTime)
{
	if (GetVelocity().SizeSquared() > 10.0f)
	{
		// Increase our running time clock
		RunningTime += DeltaTime;

		// FMath::Sin returns a smooth wave between -1.0 and 1.0
		float SineWave = FMath::Sin(RunningTime * ZigzagFrequency);

		// Multiply by our intensity to control how aggressive the dodge is
		float SidewaysInput = SineWave * ZigzagIntensity;

		// Push the character to the right (a negative value automatically pushes them left)
		FVector RightVector = GetActorRightVector();
		AddMovementInput(RightVector, SidewaysInput);
	}
	else
	{
		// Optional: Reset time when they stop so they always start the zigzag the same way next time
		RunningTime = 0.0f; 
	}
}

