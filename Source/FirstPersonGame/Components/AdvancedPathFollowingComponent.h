// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "AdvancedPathFollowingComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMovementStyle : uint8
{
	Normal      UMETA(DisplayName = "Normal Path"),
	Zigzag      UMETA(DisplayName = "Zigzag Movement")
};

UCLASS()
class FIRSTPERSONGAME_API UAdvancedPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()
public:
	UAdvancedPathFollowingComponent();

	// Expose the style so we can change it dynamically
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Movement")
	EMovementStyle CurrentMovementStyle = EMovementStyle::Normal;

protected:
	virtual void FollowPathSegment(float DeltaTime) override;

private:
	float RunningTime;

	// Zigzag parameters
	UPROPERTY(EditAnywhere, Category = "AI Movement")
	float ZigzagFrequency = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI Movement")
	float ZigzagIntensity = 1.0f;
	
	float ZigzagMovement(float DeltaTime);

	FVector CurrentLocation;
	FVector CurrentTarget;
	FVector PathDirection;
	FVector PerpendicularDir;
};
