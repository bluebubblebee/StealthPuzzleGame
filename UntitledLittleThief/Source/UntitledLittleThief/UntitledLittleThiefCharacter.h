// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utils/Definitions.h"
#include "UntitledLittleThiefCharacter.generated.h"



UCLASS(Blueprintable)
class AUntitledLittleThiefCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUntitledLittleThiefCharacter();

	virtual void BeginPlay() override;


	virtual void Tick(float DeltaSeconds) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RaycastPointComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* CarryPickupsPointComponent;

protected:

	// Noise emitter, with this the AI can detect noises from this pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UPawnNoiseEmitterComponent* NoiseEmitterComponent;

public:

	void MoveForward(float Value);

	void MoveRight(float Value);

	void SetRunMode(bool Value);
	

	////////////////// Stealth Mode //////////////////////////////

public:
	void ChangeStatus(const EStatus& NewStatus);

protected:

	EStatus CurrentStatus = EStatus::VE_NONE;

	bool bLockInput = false;

	void UpdatePickingItemStatus(float DeltaSeconds);

	void UpdateLookAtActor(float DeltaSeconds);



	////////////////// Push Mode//////////////////////////////
protected:
	
	EPushDirection PushDirection;
	EPushDirection LastPushDirection;

	float PushForce = 0.0f;

	class APushObject* PushObject;

public:

	void SetPushMode(bool Active);

	////////////////// Push Mode //////////////////////////////


private:

	// Reference to animator
	class URobotAnimInstance * Animator = nullptr;

	////////////////// Stealth Mode //////////////////////////////


public:
	void SetStealthMode(bool Value);

	//UFUNCTION(BlueprintCallable, Category ="Character Modes")
	//bool IsStealthActivated() { return bStealthActivated; }

protected:
	 

	// Indicates transition to stealth mode, and out of stealth mode
	UPROPERTY(VisibleAnywhere, Category = "Stealth")
	bool bStealthActivated = false;

	bool bTransitionToStealth = false;
	bool bTransitionOutOfStealth = false;
	float TransitionStealthSpeed = 3.0f;
	float CurrentStealthAlpha = 0.0f;

	void HandleStealthTransition(float DeltaSeconds);	

	////////////////// Stealth Mode //////////////////////////////


public:

	////////////////// Head Look At Rotation //////////////////////////////

	UPROPERTY(EditAnywhere, Category = "HeadRotation")
	FRotator CurrentHeadRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "HeadRotation")
	float AlphaHeadRotation = 1.0f;

	UPROPERTY(EditAnywhere, Category = "HeadRotation")
	float MaxLeftRightRotation = 100.0f;

	UPROPERTY(EditAnywhere, Category = "HeadRotation")
	float MaxUpDownRotation = 70.0f;

	bool bTransitionHeadOut;

	float CurrentAlphaHeadTranstion;

	////////////////// Head Look At Rotation //////////////////////////////


private:

	AActor* ActorToLookAt = nullptr;

	// Interactives
	class AInteractiveBase* CurrentInteractive;

	class APickupBase* CurrentPickup;


public:
	void SetInteractiveInRange(class AInteractiveBase* Interactive);
	void ClearInteractiveInRange(class AInteractiveBase* Interactive);

	FORCEINLINE AInteractiveBase* GetCurrentInteractive() { return CurrentInteractive;};


	////////////////// Interact //////////////////////////////
public:
	void HandleInteract();
	
	FORCEINLINE APickupBase* GetCurrentPickup() const { return CurrentPickup; };

private:

	bool bInteractingInTransition;
	bool bInteractingOutTransition;
	float CurrentAlphaValue;
	FVector CurrentLeftHandPosition;

	////////////////// Interact //////////////////////////////


	////////////////// Camera Zoom //////////////////////////////	

public:
	void ZoomIn();

	void ZoomOut();

protected:

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float InitialCameraZoom = 695.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float TargetCameraZoom = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float ZoomTransitionSpeed = 3.0f;

	float CurrentCameraZoom;

	bool bTransitionZoomIn;

	bool bTransitionZoomOut;

	void HandleZoom(float DeltaSeconds);

////////////////// Camera Zoom //////////////////////////////
	
	
	
////////////////// Animations //////////////////////////////

public:

	void Animate(const EAnimationType& Animation);


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Events")
	void OnAnimationCompleted(const EAnimationType& AnimationCompleted);

	void OnAnimationCompleted_Implementation(const EAnimationType& AnimationCompleted);

	////////////////// Animations //////////////////////////////


};

