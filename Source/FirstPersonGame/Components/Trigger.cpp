// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Trigger.h"

#include "AI/NavigationSystemBase.h"

// Sets default values for this component's properties
UTrigger::UTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (MoverActor)
	{
		Mover = MoverActor->FindComponentByClass<UMover>();
		if (!Mover)
		{
			UE_LOG(LogTemp, Display, TEXT("Trigger is missing a reference to a Mover component!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Mover actor missing!"));
	}
	
	OnComponentBeginOverlap.AddDynamic(this,&UTrigger::OverlapBegin);
	OnComponentEndOverlap.AddDynamic(this,&UTrigger::OverlapEnd);
}


// Called every frame
void UTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTrigger::Trigger(bool bIsOverlapping) const
{
	if (Mover)
	{
		Mover->setCanMove(bIsOverlapping);
	}
}

void UTrigger::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Trigger(true);
}

void UTrigger::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Trigger(false);
}

