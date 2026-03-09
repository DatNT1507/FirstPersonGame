// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FirstPersonGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class USpotLightComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AFirstPersonGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;

	// The actual flashlight component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	USpotLightComponent* Flashlight;
public:
	AFirstPersonGameCharacter();

protected:

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	// Function we can call to turn it on/off
	UFUNCTION(BlueprintCallable, Category = "Flashlight")
	void ToggleFlashlight();

	virtual void Tick(float DeltaTime) override;

	void BatteryTick(float DeltaTime);
protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Tracks whether the player has picked up the maze key
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Objective")
	bool bHasKey = false;
	
	// Tracks if the flashlight is currently in "Bright" mode
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	bool bIsFlashlightOn;

	// The current charge of the flashlight (0.0 to 10.0)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	float CurrentBattery;

	// The maximum charge (10 seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
	float MaxBattery = 10.0f;

	// The brightness when turned ON
	UPROPERTY(EditAnywhere, Category = "Flashlight")
	float BrightIntensity = 1000.0f;

	// The brightness when turned OFF (slightly bright)
	UPROPERTY(EditAnywhere, Category = "Flashlight")
	float DimIntensity = 100.0f;
};

