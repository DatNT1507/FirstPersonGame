// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/ExitZone.h"

#include "FirstPersonGameCharacter.h"
#include "Components/BoxComponent.h"
#include "FirstPersonGameGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AExitZone::AExitZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	initializeComponents();
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
		if (AFirstPersonGameGameMode* MyGameMode = Cast<AFirstPersonGameGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			// Check if they have the key
		   if (Player->bHasKey)
		   {
		   		MyGameMode->HandleGameEnd(true);
		   }
		   else
		   {
			   MyGameMode->ShowNotification(TEXT("The door is locked. Find all the keys!"));
		   }
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

