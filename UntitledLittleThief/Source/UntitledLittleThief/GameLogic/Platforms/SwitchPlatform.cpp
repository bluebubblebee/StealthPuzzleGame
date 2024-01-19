// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchPlatform.h"
#include "UntitledLittleThiefCharacter.h"
#include "MovablePlatform.h"



void ASwitchPlatform::StartInteract(APawn* PawnInstigator)
{
	Super::StartInteract(PawnInstigator);

	if ((CurrentCharacter == nullptr)|| (Platform == nullptr)) return;

	if (IsSwitchedOn)
	{
		IsSwitchedOn = false;

		Platform->SwitchOff();

		OnSwitchOn();
		
	}
	else
	{
		IsSwitchedOn = true;

		Platform->SwitchOn();

		OnSwitchOff();
		
	}
	CurrentCharacter->Animate(EAnimationType::VE_INTERACT);

}
