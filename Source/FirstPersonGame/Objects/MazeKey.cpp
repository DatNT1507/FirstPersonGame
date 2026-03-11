// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MazeKey.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FirstPersonGameCharacter.h"
#include "FirstPersonGameGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMazeKey::AMazeKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	initializeComponents();
}

// Called when the game starts or when spawned
void AMazeKey::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMazeKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the actor that overlapped is our player character
	if (AFirstPersonGameCharacter* Player = Cast<AFirstPersonGameCharacter>(OtherActor))
	{
		// Give the player the key
		Player->bHasKey = true;


		if (AFirstPersonGameGameMode* GameMode = Cast<AFirstPersonGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// Show a notification that they picked up the key
			GameMode->ShowNotification(TEXT("You picked up a key! Find the exit!"));
		}

		// Destroy the key from the world
		Destroy();
	}
}

// Called every frame
void AMazeKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeKey::initializeComponents()
{
	// Set up the collision sphere
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(50.0f);
	CollisionComp->SetCollisionProfileName("Trigger");
	RootComponent = CollisionComp;

	// Set up the visual mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// Bind the overlap event
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMazeKey::OnOverlapBegin);
}

