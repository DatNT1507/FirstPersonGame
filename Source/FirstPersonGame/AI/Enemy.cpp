// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemy.h"
#include "FirstPersonGameCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonGameGameMode.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapZone = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapZone"));
	OverlapZone->SetupAttachment(RootComponent);
	OverlapZone->InitSphereRadius(60.0f); 
	OverlapZone->SetCollisionProfileName("Trigger");

	OverlapZone->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFirstPersonGameCharacter *Player = Cast<AFirstPersonGameCharacter>(OtherActor))
	{
		if (AFirstPersonGameGameMode* MyGameMode = Cast<AFirstPersonGameGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			// Tell the GameMode the player lost!
			MyGameMode->HandleGameEnd(false); 
		}
	}
}

