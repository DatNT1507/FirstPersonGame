// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/ExitZone.h"

#include "FirstPersonGameCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AExitZone::AExitZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AExitZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AExitZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFirstPersonGameCharacter *Player = Cast<AFirstPersonGameCharacter>(OtherActor))
	{
		// Check if they have the key
		if (Player->bHasKey)
		{
			UE_LOG(LogTemp, Warning, TEXT("AExitZone::OnOverlapBegin"))
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Find the key"))
		}
	}
}

// Called every frame
void AExitZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitZone::initializeComponents()
{
	ExitTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitTrigger"));
	ExitTrigger->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	ExitTrigger->SetCollisionProfileName("Trigger");
	RootComponent = ExitTrigger;

	ExitTrigger->OnComponentBeginOverlap.AddDynamic(this, &AExitZone::OnOverlapBegin);
}

