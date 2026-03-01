// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MazeKey.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FirstPersonGameCharacter.h"

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
        
		// Print a debug message to the screen
		UE_LOG(LogTemp, Warning, TEXT("AMazeKey::OnOverlapBegin"))

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

