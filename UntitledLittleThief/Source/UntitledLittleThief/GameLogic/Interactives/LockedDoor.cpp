// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedDoor.h"
#include "UntitledLittleThiefCharacter.h"
#include "UntitledLittleThiefPlayerController.h"
#include "PickupBase.h"
#include "Utils/Definitions.h"
#include "GameLogic/Dialogue/DialogueActorComponent.h"

ALockedDoor::ALockedDoor()
{
	DialogueSystem = CreateDefaultSubobject<UDialogueActorComponent>(TEXT("DialogueSystem"));

	PrimaryActorTick.bCanEverTick = false;
}

void ALockedDoor::StartInteract(APawn* PawnInstigator)
{
	if (CurrentCharacter == nullptr) return;

	if (IsLocked)
	{
		// Check if character has item
		CurrentController = Cast<AUntitledLittleThiefPlayerController>(CurrentCharacter->GetController());

		if (CurrentController == nullptr) return;

		DialogueSystem->OnDialogueCompletedEvent.AddUniqueDynamic(this, &ALockedDoor::OnDialogueComplete);

		if (CurrentCharacter->GetCurrentPickup() != nullptr)
		{
			if (CurrentCharacter->GetCurrentPickup()->GetPickupType() == EPickupType::VE_KEYDOOR)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player has pickup good type"));

				DialogueSystem->Start(CurrentController, DialogueIDDoorUnLocked);

				Super::StartInteract(PawnInstigator);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player has pickup but wrong type"));

				//CurrentCharacter->Animate(EAnimationType::VE_DISMISS);

				CurrentController->LockInput();
				DialogueSystem->Start(CurrentController, DialogueIDDoorLocked);
				
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player doesn't have a pickup"));

			//CurrentCharacter->Animate(EAnimationType::VE_DISMISS);
			CurrentController->LockInput();
			DialogueSystem->Start(CurrentController, DialogueIDDoorLocked);
		}
	}	
}

void ALockedDoor::OnDialogueComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("ALockedDoor::OnDialogueComplete"));

	if (CurrentController != nullptr)
	{
		CurrentController->UnLockInput();
	}
}

