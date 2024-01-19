// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "UntitledLittleThiefCharacter.h"
#include "Utils/Definitions.h"


void ADoor::StartInteract(APawn* PawnInstigator)
{
	Super::StartInteract(PawnInstigator);

	if (CurrentCharacter == nullptr) return;

	if (IsOpen) return;	

	IsOpen = true;

	// Animate ve_whatever animation
	CurrentCharacter->Animate(EAnimationType::VE_INTERACT);

	CurrentCharacter->SetInteractiveInRange(nullptr);

	// Call blueprint event
	OnOpenDoor(1.0f);

	// Disable interactive
	SetEnable(false);	
}