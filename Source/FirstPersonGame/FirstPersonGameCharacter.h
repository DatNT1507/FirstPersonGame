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

	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* DoSprintAction;

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

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoSprint();

	virtual void Tick(float DeltaTime) override;

	void BatteryTick(float DeltaTime);
protected:

	// How fast the camera bounces (matches footstep speed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float SprintBobSpeed = 15.0f;

	// How intense the bounce is (how high/low it goes)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float SprintBobAmount = 5.0f;

	// Keeps track of how long we've been running to feed the Sine wave
	float RunTime = 0.0f;

	// Remembers the camera's normal Z height so we don't push it through the floor!
	float DefaultCameraZ = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 800.0f;

	// Stamina Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainRate = 20.0f; // Drains 20 per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate = 15.0f; // Regens 15 per second

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	// FOV Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float DefaultFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float SprintFOV = 110.0f;

	// How fast the camera zooms in and out
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float ZoomInterpSpeed = 10.0f;

	// Tracks whether the camera should be zooming out
	bool bIsSprinting = false;

	float FootstepTimer = 0.0f;

	// Sprint functions 
	void StartSprint();
	void StopSprint();
	void SprintFixedTick(float DeltaTime);
	
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
	float BrightIntensity = 5000.0f;

	// The brightness when turned OFF (slightly bright)
	UPROPERTY(EditAnywhere, Category = "Flashlight")
	float DimIntensity = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UPawnNoiseEmitterComponent* NoiseEmitter;
};

