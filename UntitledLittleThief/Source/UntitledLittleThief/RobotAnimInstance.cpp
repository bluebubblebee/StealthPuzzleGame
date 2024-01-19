// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotAnimInstance.h"

void URobotAnimInstance::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();

	//Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();
}

//Tick
void URobotAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{

	Super::NativeUpdateAnimation(DeltaTimeX);

	if (!OwningPawn)
	{
		return;
	}
}