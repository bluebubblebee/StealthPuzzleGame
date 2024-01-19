// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "Utils/Definitions.h"
#include "LockedDoor.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDLITTLETHIEF_API ALockedDoor : public ADoor
{
	GENERATED_BODY()

public:
	ALockedDoor();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	class UDialogueActorComponent* DialogueSystem;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Door")
	EPickupType PickupToUnlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Door")
	FName DialogueIDDoorLocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Door")
	FName DialogueIDDoorUnLocked;

protected:

	bool IsLocked = true;

	class AUntitledLittleThiefPlayerController* CurrentController;

	UFUNCTION()
	void OnDialogueComplete();

public:
	/////// Interactions /////////

	virtual void StartInteract(APawn* PawnInstigator) override;

	/////// Interactions /////////
	
};
