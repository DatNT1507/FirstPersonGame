// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FirstPersonGame.h"
#include "AI/Enemy.h"
#include "AI/Enemy_Controller.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"

AFirstPersonGameCharacter::AFirstPersonGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Initialize battery state
	MaxBattery = 10.0f;
	CurrentBattery = MaxBattery;
	bIsFlashlightOn = false;

	// Create the spotlight
	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
    
	// Attach spotlight to the camera 
	Flashlight->SetupAttachment(FirstPersonCameraComponent); 
    
	// Set default properties
	Flashlight->SetInnerConeAngle(0.0f);
	Flashlight->SetOuterConeAngle(90.0f); 
	Flashlight->SetLightColor(FLinearColor(1.0f, 1.0f, 1.0f));
    
	// Start with the light ON but DIM (slightly bright)
	Flashlight->SetIntensity(DimIntensity);
	Flashlight->SetVisibility(true); // Make sure it's always visible now!

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));
	
	if (FirstPersonCameraComponent)
	{
		DefaultCameraZ = FirstPersonCameraComponent->GetRelativeLocation().Z;
	}
}

void AFirstPersonGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFirstPersonGameCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFirstPersonGameCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonGameCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstPersonGameCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AFirstPersonGameCharacter::LookInput);

		EnhancedInputComponent->BindAction(DoSprintAction, ETriggerEvent::Triggered, this, &AFirstPersonGameCharacter::StartSprint);
		EnhancedInputComponent->BindAction(DoSprintAction, ETriggerEvent::Completed, this, &AFirstPersonGameCharacter::StopSprint);
	}
	else
	{
		UE_LOG(LogFirstPersonGame, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFirstPersonGameCharacter::StartSprint()
{
	if (CurrentStamina > 0.0f && GetVelocity().Size() > 0.0f)
	{
		bIsSprinting = true;
        
		// Physically make the character run faster
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		bIsSprinting = false;
        
		// Physically make the character run faster
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AFirstPersonGameCharacter::StopSprint()
{
	bIsSprinting = false;
    
	// Return the character to normal walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFirstPersonGameCharacter::SprintFixedTick(float DeltaTime)
{
	if (bIsSprinting)
    {
        // Only drain stamina if the player is actually moving (not just holding the key against a wall)
        if (GetVelocity().Size() > 0.0f)
        {
            CurrentStamina -= StaminaDrainRate * DeltaTime;
            CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);

        	MakeNoise(1.0f, this, GetActorLocation());
        	
            // Force the player to stop sprinting if stamina hits 0
            if (CurrentStamina <= 0.0f)
            {
                StopSprint();
            }
        }
    }
    else
    {
        // Regenerate stamina when not sprinting
        if (CurrentStamina < MaxStamina)
        {
            CurrentStamina += StaminaRegenRate * DeltaTime;
            CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
        }
    }
	
    if (FirstPersonCameraComponent)
    {
        // Smooth FOV Zoom
        float TargetFOV = bIsSprinting ? SprintFOV : DefaultFOV;
        float CurrentFOV = FirstPersonCameraComponent->FieldOfView;
        float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, ZoomInterpSpeed);
        FirstPersonCameraComponent->SetFieldOfView(NewFOV);

        // Procedural Head Bobbing
        if (bIsSprinting && GetVelocity().Size() > 0.0f)
        {
            // Increase the wave timer
            RunTime += DeltaTime;

            // Calculate the up-and-down bounce using a Sine wave
            float BobOffset = FMath::Sin(RunTime * SprintBobSpeed) * SprintBobAmount;

            // Apply the bounce
            FVector NewLocation = FirstPersonCameraComponent->GetRelativeLocation();
            NewLocation.Z = DefaultCameraZ + BobOffset;
            FirstPersonCameraComponent->SetRelativeLocation(NewLocation);
        }
        else
        {
            // Smoothly return the camera back to its normal resting height when we stop
            RunTime = 0.0f; 
            FVector CurrentLocation = FirstPersonCameraComponent->GetRelativeLocation();
            CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, DefaultCameraZ, DeltaTime, ZoomInterpSpeed);
            FirstPersonCameraComponent->SetRelativeLocation(CurrentLocation);
        }
    }
}


void AFirstPersonGameCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AFirstPersonGameCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AFirstPersonGameCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AFirstPersonGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AFirstPersonGameCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AFirstPersonGameCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void AFirstPersonGameCharacter::ToggleFlashlight()
{
	if (!Flashlight) return;

	// Turn ON: Only if it's currently off AND we have some battery left
	if (!bIsFlashlightOn && CurrentBattery > 0.0f)
	{
		bIsFlashlightOn = true;
		Flashlight->SetIntensity(BrightIntensity);
		UE_LOG(LogTemp, Warning, TEXT("Flashlight turned ON"));
	}
	// Turn OFF: If it's currently on, turn it off manually
	else if (bIsFlashlightOn)
	{
		bIsFlashlightOn = false;
		Flashlight->SetIntensity(DimIntensity);
		UE_LOG(LogTemp, Warning, TEXT("Flashlight turned OFF"));
	}
}

void AFirstPersonGameCharacter::DoSprint()
{
}

void AFirstPersonGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BatteryTick(DeltaTime);
	SprintFixedTick(DeltaTime);
}

void AFirstPersonGameCharacter::BatteryTick(float DeltaTime)
{
	// If the flashlight is ON, drain the battery
	if (bIsFlashlightOn)
	{
		CurrentBattery -= DeltaTime;

		// Force it to turn OFF if the battery hits 0
		if (CurrentBattery <= 0.0f)
		{
			CurrentBattery = 0.0f;
			bIsFlashlightOn = false;
			Flashlight->SetIntensity(DimIntensity); // Drop back to slightly bright
            
			UE_LOG(LogTemp, Warning, TEXT("Battery dead! Flashlight forced off."));
		}
	}
	// If the flashlight is OFF, recharge the battery
	else 
	{
		if (CurrentBattery < MaxBattery)
		{
			CurrentBattery += DeltaTime;
            
			// Cap it at the maximum battery (10.0f)
			if (CurrentBattery > MaxBattery)
			{
				CurrentBattery = MaxBattery;
			}
		}
	}
}
