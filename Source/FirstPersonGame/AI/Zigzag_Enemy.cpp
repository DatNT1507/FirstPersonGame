// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Zigzag_Enemy.h"

// Sets default values
AZigzag_Enemy::AZigzag_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}

// Called to bind functionality to input
void AZigzag_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

