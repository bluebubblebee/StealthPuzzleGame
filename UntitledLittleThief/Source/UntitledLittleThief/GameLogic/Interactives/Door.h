// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveBase.h"

#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDLITTLETHIEF_API ADoor : public AInteractiveBase
{
	GENERATED_BODY()

protected:

	bool IsOpen = false;

	// Blueprint Event to open door
	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void OnOpenDoor(float Delay);


public:
	/////// Interactions /////////

	virtual void StartInteract(APawn* PawnInstigator);

	/////// Interactions /////////
	
};
