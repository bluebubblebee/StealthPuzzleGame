// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UntitledLittleThiefCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "UntitledLittleThiefPlayerController.h"
#include "RobotAnimInstance.h"

#include "GameLogic/Interactives/InteractiveBase.h"
#include "GameLogic/Interactives/PickupBase.h"
#include "GameLogic/Interactives/PushObject.h"

#include "Utils/Definitions.h"

#include "Materials/Material.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AUntitledLittleThiefCharacter::AUntitledLittleThiefCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f); 

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	//CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));   
	CameraBoom->TargetArmLength = 800.f;

	//CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm	

	RaycastPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RaycastPointComponent"));
	RaycastPointComponent->SetupAttachment(RootComponent);

	CarryPickupsPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CarryPickupsPointComponent"));
	CarryPickupsPointComponent->SetupAttachment(RootComponent);

	// Create instance of Noise Emitter component
	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AUntitledLittleThiefCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetMesh() != nullptr)
	{
		Animator = Cast<URobotAnimInstance>(GetMesh()->GetAnimInstance());
	}

	CurrentHeadRotation = FRotator::ZeroRotator;

	// Set initial camera zoom
	CameraBoom->TargetArmLength = InitialCameraZoom;


	CurrentStatus = EStatus::VE_NONE;
}

void AUntitledLittleThiefCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	
	// Handle Stealth
	HandleStealthTransition(DeltaSeconds);	

	// Look at actor
	UpdateLookAtActor(DeltaSeconds);

	// Handle Pickup object with IK
	if (CurrentStatus == EStatus::VE_PICKING_ITEM)
	{
		UpdatePickingItemStatus(DeltaSeconds);
	}

	// Carrying item
	if ((CurrentStatus == EStatus::VE_CARRYING_ITEM)/* && (CurrentPickup != nullptr)*/)
	{
		// Transition to removed alpha on hands
		if (bInteractingOutTransition)
		{
			CurrentAlphaValue -= (4 * DeltaSeconds);

			

			Animator->SetLeftHandIKAlpha(CurrentAlphaValue);
			Animator->SetRightHandIKAlpha(CurrentAlphaValue);

			if (CurrentAlphaValue <= 0.0f)
			{
				CurrentAlphaValue = 0.0f;

				Animator->SetLeftHandIKAlpha(CurrentAlphaValue);
				Animator->SetRightHandIKAlpha(CurrentAlphaValue);


				bInteractingOutTransition = false;

				CurrentStatus = EStatus::VE_NONE;
			}
		}
	}

	// Push 
	if ((CurrentStatus == EStatus::VE_PUSHING) && (PushObject != nullptr))
	{
		if (LastPushDirection != PushDirection)
		{
			SetPushMode(false);
			PushForce = 0.0f;
		}
		else
		{
			SetPushMode(true);
			PushForce = 200.0f;

			PushObject->Push(GetActorForwardVector(), PushForce, DeltaSeconds);
		}		
	}

	// Handle Zoom In, Zoom Out
	HandleZoom(DeltaSeconds);
	
}

void AUntitledLittleThiefCharacter::UpdateLookAtActor(float DeltaSeconds)
{
	if (ActorToLookAt != nullptr)
	{
		// Get the head socket location of the mesh (world space)
		FVector HeadSocketLocation = GetMesh()->GetSocketLocation("HeadSocket");

		// Finds the rotation from the start point (headSocket location) to the target point (the interactive location)
		// Not taking into account my rotation
		FVector LookAtLocation = ActorToLookAt->GetActorLocation();
		if (CurrentInteractive != nullptr)
		{
			LookAtLocation = CurrentInteractive->GetLookAtPoint()->GetComponentLocation();
		}
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HeadSocketLocation, LookAtLocation);
		//FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HeadSocketLocation, ActorToLookAt->GetActorLocation());

		// Apply the actor rotation to the look at to align both rotators
		FRotator RotatorDifference = FRotator(
			LookAtRotation.Pitch - GetActorRotation().Pitch,
			LookAtRotation.Yaw - GetActorRotation().Yaw,
			0.0f);
		//UE_LOG(LogTemp, Warning, TEXT("RotatorDifference (%f, %f, %f )"), RotatorDifference.Roll, RotatorDifference.Pitch, RotatorDifference.Yaw);

		// Convert the final rotation to the local space for the Head
		// I am feeding the rotation in local bone space.
		// The head bone is not aligned with UE Space, it is necesary to reconvert
		FRotator TargetRotation = FRotator(RotatorDifference.Yaw, 0.0f, -RotatorDifference.Pitch);
		//UE_LOG(LogTemp, Warning, TEXT("TargetRotation (%f, %f, %f )"), TargetRotation.Roll, TargetRotation.Pitch, TargetRotation.Yaw);

		// Restrict limits (Left-Right limit)
		if (FMath::Abs(TargetRotation.Pitch) > MaxLeftRightRotation)
		{
			TargetRotation.Pitch = 0.0f;
		}

		// Restrict limits (Up-Down limit)
		if (FMath::Abs(TargetRotation.Yaw) > MaxUpDownRotation)
		{
			TargetRotation.Yaw = 0.0f;
		}

		// Lerp the rotation and set it on the animator
		CurrentHeadRotation = FMath::RInterpTo(CurrentHeadRotation, TargetRotation, DeltaSeconds, 4.0f);
		Animator->SetRotationHead(CurrentHeadRotation);

	}
	else if (bTransitionHeadOut)
	{
		CurrentHeadRotation = FMath::RInterpTo(CurrentHeadRotation, FRotator::ZeroRotator, DeltaSeconds, 4.0f);
		Animator->SetRotationHead(CurrentHeadRotation);

		if (CurrentHeadRotation.IsNearlyZero())
		{
			CurrentHeadRotation = FRotator::ZeroRotator;
			Animator->SetRotationHead(CurrentHeadRotation);

			bTransitionHeadOut = false;
		}
	}
}

void AUntitledLittleThiefCharacter::UpdatePickingItemStatus(float DeltaSeconds)
{
	if (bInteractingInTransition) 
	{
		CurrentAlphaValue += (4 * DeltaSeconds);

		Animator->SetLeftHandIKAlpha(CurrentAlphaValue);

		Animator->SetLeftHandTargetLocator(CurrentInteractive->GetActorLocation());

		if (CurrentAlphaValue >= 1.0f)
		{
			CurrentAlphaValue = 1.0f;

			Animator->SetLeftHandIKAlpha(CurrentAlphaValue);

			CurrentInteractive->SetEnable(false);


			bInteractingInTransition = false;

			FVector SocketLocation = GetMesh()->GetSocketLocation("LeftHandSocket");
			FRotator SocketRotation = GetMesh()->GetSocketRotation("LeftHandSocket");


			CurrentPickup = GetWorld()->SpawnActor<APickupBase>(CurrentInteractive->PickupClass, SocketLocation, SocketRotation);

			if (CurrentPickup != nullptr)
			{
				// Set both arms holding box in front of them
				if (CurrentPickup->GetPickupType() == EPickupType::VE_PRESSUREBOX)
				{
					CurrentPickup->AttachToComponent(CarryPickupsPointComponent, FAttachmentTransformRules::KeepWorldTransform);

					CurrentPickup->SetActorLocation(CarryPickupsPointComponent->GetComponentLocation());
					CurrentPickup->SetActorRotation(CarryPickupsPointComponent->GetComponentRotation());

					Animator->SetLeftHandIKAlpha(1.0f);
					Animator->SetRightHandIKAlpha(1.0f);

					FVector LeftTarget(100.0f, 180.0f, 260.0f);
					FVector RigthTarget(-100.0f, 180.0f, 260.0f);


					Animator->SetLeftHandTargetLocator(LeftTarget);
					Animator->SetRightHandTargetLocator(RigthTarget);

					CurrentStatus = EStatus::VE_CARRYING_ITEM;

					// Walk slower
					GetCharacterMovement()->MaxWalkSpeed = 320;


				}
				else if (CurrentPickup->GetPickupType() == EPickupType::VE_KEYDOOR)
				{
					CurrentPickup->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("LeftHandSocket"));

					CurrentStatus = EStatus::VE_CARRYING_ITEM;

					bInteractingOutTransition = true;


				}
			}

			CurrentInteractive->GetInteractiveMesh()->SetVisibility(false, true);

			bTransitionHeadOut = true;
			ActorToLookAt = nullptr;
			CurrentInteractive = nullptr;
		}
	}
	else if (bInteractingOutTransition)
	{
		CurrentAlphaValue -= (4 * DeltaSeconds);

		Animator->SetLeftHandIKAlpha(CurrentAlphaValue);

		if (CurrentAlphaValue <= 0.0f)
		{
			CurrentAlphaValue = 0.0f;

			Animator->SetLeftHandIKAlpha(CurrentAlphaValue);

			bInteractingOutTransition = false;

			CurrentStatus = EStatus::VE_NONE;

		}
	}
}


////////////////// MOVEMENT ////////////////////////////////////

void AUntitledLittleThiefCharacter::MoveForward(float Value)
{
	if (bLockInput) return;

	if (Value != 0.0f)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);

		// Check push status
		if (CurrentStatus == EStatus::VE_PUSHING)
		{
			PushForce = 0.0f;

			if (Value > 0.0f)
			{
				LastPushDirection = EPushDirection::VE_TO_LEFT;
			}
			else if (Value < 0.0f)
			{
				LastPushDirection = EPushDirection::VE_TO_RIGHT;
			}
			else
			{
				LastPushDirection = EPushDirection::VE_NONE;
			}

			if ((PushDirection == EPushDirection::VE_TO_LEFT) || (PushDirection == EPushDirection::VE_TO_RIGHT))
			{
				// Only enable push if the direction is correct and corresponds with the direction of push
				if ((PushDirection == EPushDirection::VE_TO_LEFT) && (Value > 0.0f))
				{
					
					PushForce = 200.0f;
				}
				else if ((PushDirection == EPushDirection::VE_TO_RIGHT) && (Value < 0.0f))
				{					
					PushForce = 200.0f;
				}
			}
		}
	}

}

void AUntitledLittleThiefCharacter::MoveRight(float Value)
{
	if (bLockInput) return;

	if (Value != 0.0f)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
		

		// Check push status
		if (CurrentStatus == EStatus::VE_PUSHING)
		{
			PushForce = 0.0f;

			if (Value > 0.0f)
			{
				LastPushDirection = EPushDirection::VE_TO_DOWN;
			}
			else if (Value < 0.0f)
			{
				LastPushDirection = EPushDirection::VE_TO_TOP;
			}
			else
			{
				LastPushDirection = EPushDirection::VE_NONE;
			}

			// Only enable push if the direction is correct and corresponds with the direction of push
			if ((PushDirection == EPushDirection::VE_TO_DOWN) || (PushDirection == EPushDirection::VE_TO_TOP))
			{
				if ((PushDirection == EPushDirection::VE_TO_DOWN) && (Value > 0.0f))
				{				
					LastPushDirection = EPushDirection::VE_TO_DOWN;
					PushForce = 50.0f;
				}
				else if ((PushDirection == EPushDirection::VE_TO_TOP) && (Value < 0.0f))
				{
					LastPushDirection = EPushDirection::VE_TO_TOP;
					PushForce = 50.0f;					
				}
			}
		}
	}

}

void AUntitledLittleThiefCharacter::SetRunMode(bool Value)
{
	if (bLockInput) return;

	if (CurrentStatus != EStatus::VE_NONE) return;

	if (bStealthActivated) return;

	if (Value)
	{
		GetCharacterMovement()->MaxWalkSpeed = 750;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
}

//////////////////  MOVEMENT  ////////////////////////////////////




////////////////// Stealth Mode ////////////////////////////////////

void AUntitledLittleThiefCharacter::SetStealthMode(bool Value)
{
	if (bLockInput) return;

	if (CurrentStatus != EStatus::VE_NONE) return;

	if (Animator == nullptr) return;	

	bStealthActivated = Value;

	if (Value)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400;

		bTransitionToStealth = true;

		bTransitionOutOfStealth = false;

		CurrentStealthAlpha = 0.0f;

		Animator->SetStealhValue(CurrentStealthAlpha);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;

		bTransitionOutOfStealth = true;

		bTransitionToStealth = false;

		CurrentStealthAlpha = 1.0f;

		Animator->SetStealhValue(CurrentStealthAlpha);
	}
}

void AUntitledLittleThiefCharacter::HandleStealthTransition(float DeltaSeconds)
{
	if (Animator == nullptr) return;

	// Transition to stealth
	if (bTransitionToStealth)
	{
		Animator->SetStealhValue(CurrentStealthAlpha);

		CurrentStealthAlpha += (TransitionStealthSpeed * DeltaSeconds);

		if (CurrentStealthAlpha >= 1.0f)
		{
			Animator->SetStealhValue(1.0f);

			CurrentStealthAlpha = 1.0f;
			bTransitionToStealth = false;
		}
	}

	// Transition out of stealth
	if (bTransitionOutOfStealth)
	{
		Animator->SetStealhValue(CurrentStealthAlpha);

		CurrentStealthAlpha -= (TransitionStealthSpeed * DeltaSeconds);

		if (CurrentStealthAlpha <= 0.0f)
		{
			Animator->SetStealhValue(0.0f);

			CurrentStealthAlpha = 0.0f;
			bTransitionOutOfStealth = false;
		}
	}
}

////////////////// Stealth Mode ////////////////////////////////////




////////////////// INTERACTIVES ////////////////////////////////////

void AUntitledLittleThiefCharacter::ClearInteractiveInRange(class AInteractiveBase* Interactive)
{
	if (Interactive != nullptr)
	{
		ActorToLookAt = nullptr;

		if (Interactive->CanLookAt())
		{
			bTransitionHeadOut = true;
		}	
		// Automatic zoom when the interactive in range
		if (Interactive->ZoomWhenInRange())
		{
			ZoomOut();
		}
		// Clear Push object
		PushObject = Cast<APushObject>(Interactive);

		if (PushObject)
		{
			SetPushMode(false);

			CurrentStatus = EStatus::VE_NONE;

			LastPushDirection = EPushDirection::VE_NONE;
		}		

		CurrentInteractive = nullptr;
	}
}

void AUntitledLittleThiefCharacter::SetInteractiveInRange(class AInteractiveBase* Interactive)
{
	if (Interactive != nullptr)
	{
		CurrentInteractive = Interactive;

		if (Interactive->CanLookAt())
		{
			bTransitionHeadOut = false;			

			ActorToLookAt = Interactive;

			AlphaHeadRotation = 1.0f;

			Animator->SetRotationHead(CurrentHeadRotation);

			Animator->SetRotationHeadAlpha(AlphaHeadRotation);
		}
		else
		{
			ActorToLookAt = nullptr;
		}

		// Automatic ZoomIn
		if (Interactive->ZoomWhenInRange())
		{
			ZoomIn();
		}


		// Check if it's a push object 
		PushObject = Cast<APushObject>(Interactive);		

		if (PushObject)
		{
			// Raycast
			FHitResult OutHit;
			FVector Start = RaycastPointComponent->GetComponentLocation();

			FVector End = ((GetActorForwardVector()* 100.f) + Start);
			FCollisionQueryParams CollisionParams;

			//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 5);
			
			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
			{
				APushObject* TempPushObject = Cast<APushObject>(OutHit.GetActor());

				if (TempPushObject)
				{
					float dotForward = FMath::Clamp(FVector::DotProduct(GetActorForwardVector(), PushObject->GetActorForwardVector()), 0.0f, 1.0f);
					float dotBack = FMath::Clamp(FVector::DotProduct(GetActorForwardVector(), -PushObject->GetActorForwardVector()), 0.0f, 1.0f);
					float dotRight = FMath::Clamp(FVector::DotProduct(GetActorForwardVector(), PushObject->GetActorRightVector()), 0.0f, 1.0f);
					float dotLeft = FMath::Clamp(FVector::DotProduct(GetActorForwardVector(), -PushObject->GetActorRightVector()), 0.0f, 1.0f);

					// Get the max to get the right directon to push
					if ((dotForward > dotBack) && (dotForward > dotRight) && ((dotForward > dotLeft)))
					{
						
						PushDirection = EPushDirection::VE_TO_DOWN;
						
					}

					else if ((dotBack > dotForward) && (dotBack > dotRight) && ((dotBack > dotLeft)))
					{
						PushDirection = EPushDirection::VE_TO_TOP;
						
					}

					else if ((dotRight > dotForward) && (dotRight > dotBack) && ((dotRight > dotLeft)))
					{

						PushDirection = EPushDirection::VE_TO_LEFT;
						
					}

					else if ((dotLeft > dotForward) && (dotLeft > dotBack) && ((dotLeft > dotRight)))
					{
						PushDirection = EPushDirection::VE_TO_RIGHT;
						
					}
					
					LastPushDirection = PushDirection;

					CurrentStatus = EStatus::VE_PUSHING;
				}
			}
		}
	}
	
}

////////////////// INTERACTIVES ////////////////////////////////////


void AUntitledLittleThiefCharacter::HandleInteract()
{
	// Check if animation is  none
	if (Animator == nullptr) return;

	if (bLockInput) return;

	//if (CurrentStatus != EStatus::VE_NONE) return;


	if ((CurrentStatus == EStatus::VE_CARRYING_ITEM)) 	// Carrying item
	{
		if ((CurrentInteractive != nullptr) && (CurrentPickup != nullptr))
		{		

			if (CurrentInteractive->GetSpectedItem() == EPickupType::VE_NONE) // Check if the interactive is expecting an item
			{
				UE_LOG(LogTemp, Warning, TEXT("[AUntitledLittleThiefCharacter::HandleInteract] Other interactive not expectign item"));
			}
			else
			{
				if (CurrentInteractive->GetSpectedItem() == CurrentPickup->GetPickupType()) // Same pickup
				{
					UE_LOG(LogTemp, Warning, TEXT("[AUntitledLittleThiefCharacter::HandleInteract] Same pickup"));

					switch (CurrentPickup->GetPickupType())
					{
					case EPickupType::VE_KEYDOOR:
						UE_LOG(LogTemp, Warning, TEXT("[AUntitledLittleThiefCharacter::HandleInteract] EPickupType::VE_KEYDOOR"));
						break;

					case EPickupType::VE_PRESSUREBOX:

						UE_LOG(LogTemp, Warning, TEXT("[AUntitledLittleThiefCharacter::HandleInteract] EPickupType::VE_PRESSUREBOX"));

						CurrentInteractive->StartInteract(this);

						// Remove pickup
						// Walk normal
						// Complete action with interactive
						CurrentPickup->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
						CurrentPickup->Destroy();
						CurrentPickup = nullptr;

						bInteractingOutTransition = true;

						// Go back to end
						GetCharacterMovement()->MaxWalkSpeed = 600;

						break;
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[AUntitledLittleThiefCharacter::HandleInteract] Different pickup"));
				}
			}
		}
	}


	else if (CurrentStatus == EStatus::VE_NONE)
	{
		// Check if we have the interactive in range and that interactive has a pickup
		if ((CurrentInteractive != nullptr) && (CurrentInteractive->PickupClass != nullptr))
		{
			if (CurrentPickup == nullptr)
			{
				CurrentAlphaValue = 0.0f;
				CurrentStatus = EStatus::VE_PICKING_ITEM;
				bInteractingInTransition = true;
				bInteractingOutTransition = false;
			}

			//Automatic ZoomOut
			if (CurrentInteractive->ZoomWhenInRange())
			{
				ZoomOut();
			}
		}
		else if (CurrentInteractive != nullptr)
		{
			CurrentInteractive->StartInteract(this);

			// Automatic ZoomOut
			if (CurrentInteractive->ZoomWhenInRange())
			{
				ZoomOut();
			}
		}
	}
}

////////////////// Animations //////////////////////////////

void AUntitledLittleThiefCharacter::ChangeStatus(const EStatus& NewStatus)
{
	FString CurrentStatusStr = GetEnumValueAsString<EStatus>("EStatus", CurrentStatus);
	FString ToStatusStr = GetEnumValueAsString<EStatus>("EStatus", NewStatus);

	UE_LOG(LogTemp, Warning, TEXT("[AUntitledLittleThiefCharacter::ChangeStatus] From: %s -  To: %s"), *CurrentStatusStr, *ToStatusStr);

	if (CurrentStatus == NewStatus) return;

	CurrentStatus = NewStatus;

	if (CurrentStatus == EStatus::VE_FALLING)
	{
		bLockInput = true;

		Animate(EAnimationType::VE_FALL);
	}
}


void AUntitledLittleThiefCharacter::Animate(const EAnimationType& Animation)
{
	if (Animator == nullptr) return;

	//if (Animator->GetAnimation() != EAnimationType::VE_NONE) return;
	FString CurrentAnimationStr = GetEnumValueAsString<EAnimationType>("EAnimationType", Animator->GetAnimation());

	FString ToAnimationStr = GetEnumValueAsString<EAnimationType>("EAnimationType", Animation);

	UE_LOG(LogTemp, Warning, TEXT("[AUntitledLittleThiefCharacter::Animate] From: %s -  To: %s"), *CurrentAnimationStr, *ToAnimationStr);


	Animator->SetAnimation(Animation);
}


void AUntitledLittleThiefCharacter::OnAnimationCompleted_Implementation(const EAnimationType& AnimationCompleted)
{
	if (Animator == nullptr) return;

	Animator->SetAnimation(EAnimationType::VE_NONE); 

	// Set to None
	FString ToAnimationStr = GetEnumValueAsString<EAnimationType>("EAnimationType", AnimationCompleted);

	UE_LOG(LogTemp, Warning, TEXT("[AUntitledLittleThiefCharacter::OnAnimationCompleted] %s"), *ToAnimationStr);
	AUntitledLittleThiefPlayerController* PC = Cast<AUntitledLittleThiefPlayerController>(GetController());

	switch (AnimationCompleted)
	{
		case EAnimationType::VE_DISMISS:			

			if (PC != nullptr)
			{
				PC->UnLockInput();
			}

		break;
	}	

	
}

////////////////// Animations //////////////////////////////


void AUntitledLittleThiefCharacter::SetPushMode(bool Active)
{
	if (Active)
	{
		GetCharacterMovement()->MaxWalkSpeed = 200;
		Animator->SetAnimation(EAnimationType::VE_PUSH);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
		Animator->SetAnimation(EAnimationType::VE_NONE);
	}

}


//////////////////  Camera Zoom  //////////////////////////////

void AUntitledLittleThiefCharacter::ZoomIn()
{
	bTransitionZoomOut = false;
	bTransitionZoomIn = true;
	CurrentCameraZoom = 0.0f;
}

void AUntitledLittleThiefCharacter::ZoomOut()
{
	bTransitionZoomOut = true;
	bTransitionZoomIn = false;
	CurrentCameraZoom = 0.0f;
	
}

void AUntitledLittleThiefCharacter::HandleZoom(float DeltaSeconds)
{
	if (bTransitionZoomIn)
	{
		CurrentCameraZoom += (ZoomTransitionSpeed * DeltaSeconds);

		CameraBoom->TargetArmLength -= CurrentCameraZoom;

		if (CameraBoom->TargetArmLength <= TargetCameraZoom)
		{
			CameraBoom->TargetArmLength = TargetCameraZoom;
			bTransitionZoomIn = false;
		}
	}

	if (bTransitionZoomOut)
	{
		CurrentCameraZoom += (ZoomTransitionSpeed * DeltaSeconds);

		CameraBoom->TargetArmLength += CurrentCameraZoom;

		if (CameraBoom->TargetArmLength >= InitialCameraZoom)
		{
			CameraBoom->TargetArmLength = InitialCameraZoom;
			bTransitionZoomOut = false;
		}
	}
}

//////////////////  Camera Zoom  //////////////////////////////

