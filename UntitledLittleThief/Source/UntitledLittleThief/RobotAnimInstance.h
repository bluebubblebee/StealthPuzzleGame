// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Utils/Definitions.h"
#include "RobotAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDLITTLETHIEF_API URobotAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	APawn * OwningPawn;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;


	////////////////// Stealth Mode //////////////////////////////

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float StealthValue;

public:

	FORCEINLINE void SetStealhValue(float Value) { StealthValue = Value; }

	////////////////// HStealth Mode //////////////////////////////


	////////////////// Push Mode //////////////////////////////

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float PushValue;

public:

	FORCEINLINE void SetPushValue(float Value) { PushValue = Value; }

	////////////////// HStealth Mode //////////////////////////////


	////////////////// Head Look At Rotation //////////////////////////////	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	FRotator RotationHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float RotationHeadAlpha;

public:

	FORCEINLINE void SetRotationHead(FRotator Value) { RotationHead = Value; }

	FORCEINLINE void SetRotationHeadAlpha(float Value) { RotationHeadAlpha = Value; }

	////////////////// Head Look At Rotation //////////////////////////////

	////////////////// Left Arm IK //////////////////////////////

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	FVector LeftHandTargetLocator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float LeftHandIKAlpha;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	FVector RightHandTargetLocator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float RightHandIKAlpha;

public:

	FORCEINLINE void SetLeftHandTargetLocator(FVector Value) { LeftHandTargetLocator = Value; }

	FORCEINLINE void SetLeftHandIKAlpha(float Value) { LeftHandIKAlpha = Value; }

	FORCEINLINE void SetRightHandTargetLocator(FVector Value) { RightHandTargetLocator = Value; }

	FORCEINLINE void SetRightHandIKAlpha(float Value) { RightHandIKAlpha = Value; }

	////////////////// Left Arm IK //////////////////////////////



protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Settings")
	EAnimationType CurrentAnimation;

public:
	FORCEINLINE void SetAnimation(EAnimationType Value) { CurrentAnimation = Value; }

	FORCEINLINE EAnimationType GetAnimation() { return CurrentAnimation; }
};
