// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Jump_Enemy.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AJump_Enemy::AJump_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AJump_Enemy::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(JumpTimerHandle, this, &AJump_Enemy::PerformJump, JumpInterval, true);
}

// Called every frame
void AJump_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJump_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AJump_Enemy::PerformJump()
{
	if (!GetCharacterMovement()->IsFalling() && GetVelocity().SizeSquared() > 10.0f)
	{
		Jump();
	}
}

