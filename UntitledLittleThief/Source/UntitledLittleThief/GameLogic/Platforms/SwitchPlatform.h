// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interactives/InteractiveBase.h"

#include "SwitchPlatform.generated.h"

UCLASS()
class UNTITLEDLITTLETHIEF_API ASwitchPlatform : public AInteractiveBase
{
	GENERATED_BODY()


protected:

	bool IsSwitchedOn = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Switch Platform")
	void OnSwitchOn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Switch Platform")
	void OnSwitchOff();

public:

	UPROPERTY(EditInstanceOnly, Category = "Switch Platform Settings")
	class AMovablePlatform* Platform;

	/////// Interactions /////////

	virtual void StartInteract(APawn* PawnInstigator);

	/////// Interactions /////////

};
