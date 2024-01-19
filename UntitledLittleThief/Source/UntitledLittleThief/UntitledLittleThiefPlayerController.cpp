// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UntitledLittleThiefPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UntitledLittleThiefCharacter.h"
#include "RobotAnimInstance.h"
#include "UI/PlayerInGameUI.h"


#include "GameLogic/Interactives/InteractiveBase.h"

#include "Engine/World.h"

#include "UObject/ConstructorHelpers.h"

AUntitledLittleThiefPlayerController::AUntitledLittleThiefPlayerController()
{
	// Find Widget UI Blueprint clas
	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/Game/UI/WBP_InGameUI"));

	if (InGameMenuBPClass.Class != nullptr)
	{
		InGameUIClass = InGameMenuBPClass.Class;
	}
}


void AUntitledLittleThiefPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to the character controllerd by this player controller
	APawn* const pawn = GetPawn();
	if (pawn != nullptr)
	{
		MyCharacter = Cast<AUntitledLittleThiefCharacter>(pawn);
	}	

	// Create InGameUI and add it to the viewpoert
	if (InGameUIClass != nullptr)
	{
		InGameUI = CreateWidget<UPlayerInGameUI>(this, InGameUIClass);
	}

	if (InGameUI != nullptr)
	{
		InGameUI->AddToViewport();

	}

	//bIsInputLocked = false;

}


void AUntitledLittleThiefPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Input controller
	InputComponent->BindAxis("MoveForward", this, &AUntitledLittleThiefPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUntitledLittleThiefPlayerController::MoveRight);

	InputComponent->BindAction("Run", IE_Pressed, this, &AUntitledLittleThiefPlayerController::EnableRunMode);
	InputComponent->BindAction("Run", IE_Released, this, &AUntitledLittleThiefPlayerController::DisableRunMode);

	// Zoom input
	InputComponent->BindAction("Zoom", IE_Pressed, this, &AUntitledLittleThiefPlayerController::ZoomIn);
	InputComponent->BindAction("Zoom", IE_Released, this, &AUntitledLittleThiefPlayerController::ZoomOut);


	// Face button	
	InputComponent->BindAction("Interact", IE_Pressed, this, &AUntitledLittleThiefPlayerController::Interact);

	InputComponent->BindAction("StealthMode", IE_Pressed, this, &AUntitledLittleThiefPlayerController::EnableStealthMode);
	InputComponent->BindAction("StealthMode", IE_Released, this, &AUntitledLittleThiefPlayerController::DisableStealthMode);

}

void AUntitledLittleThiefPlayerController::LockInput() 
{ 
	bIsInputLocked = true; 
}

void AUntitledLittleThiefPlayerController::UnLockInput()
{
	bIsInputLocked = false;
}


void AUntitledLittleThiefPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);


	if (blockNavigateUI)
	{
		lockNavigateUIDeltaTime += DeltaTime;

		if (lockNavigateUIDeltaTime >= 0.3f)
		{
			blockNavigateUI = false;
		}

	}
}


void AUntitledLittleThiefPlayerController::EnableRunMode()
{
	if (bIsInputLocked) return;

	MyCharacter->SetRunMode(true);
}

void AUntitledLittleThiefPlayerController::DisableRunMode()
{
	if (bIsInputLocked) return;

	MyCharacter->SetRunMode(false);
}


void AUntitledLittleThiefPlayerController::EnableStealthMode()
{
	if (bIsInputLocked) return;

	MyCharacter->SetStealthMode(true);
}

void AUntitledLittleThiefPlayerController::DisableStealthMode()
{
	if (bIsInputLocked) return;

	MyCharacter->SetStealthMode(false);
}

void AUntitledLittleThiefPlayerController::MoveForward(float Value)
{
	if (bUIActive)
	{
		if (blockNavigateUI) return;

		if (Value >= 0.2f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MoveForward UP"));

			blockNavigateUI = true;
			lockNavigateUIDeltaTime = 0.0f;
			OnNavigateUI.Broadcast(EInputDirection::VE_DOWN);
		}
		else if (Value < -0.2f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MoveForward DOIWN"));
			blockNavigateUI = true;
			lockNavigateUIDeltaTime = 0.0f;
			OnNavigateUI.Broadcast(EInputDirection::VE_UP);
		}
	}


	if (bIsInputLocked) return;
	
	if (MyCharacter != nullptr)
	{
		MyCharacter->MoveForward(Value);
	}
	
}

void AUntitledLittleThiefPlayerController::MoveRight(float Value)
{
	if (bUIActive)
	{
		if (blockNavigateUI) return;

		if (Value >= 0.2f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MoveForward RIGHT"));
			blockNavigateUI = true;
			lockNavigateUIDeltaTime = 0.0f;
			OnNavigateUI.Broadcast(EInputDirection::VE_RIGHT);
		}
		else if (Value < -0.2f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MoveForward LEFT"));
			blockNavigateUI = true;
			lockNavigateUIDeltaTime = 0.0f;
			OnNavigateUI.Broadcast(EInputDirection::VE_LEFT);
		}
	}


	if (bIsInputLocked) return;
	
	if (MyCharacter != nullptr)
	{
		MyCharacter->MoveRight(Value);
	}	
}


void AUntitledLittleThiefPlayerController::Interact()
{
	if ((MyCharacter == nullptr) || (InGameUI == nullptr)) return;

	// If showing UI
	if (bUIActive)
	{
		OnInteract.Broadcast();

		return;
	}
	MyCharacter->HandleInteract();


	/*

	if (!UICameraActive)
	{
		// ONLY WHEN INTERACTING WITH SOMETHING

		if (bIsInputLocked) return;

		bIsInputLocked = true;

		// Check if there is an interactive and has messages
		bUIActive = false;
		if (MyCharacter->GetCurrentInteractive() != nullptr)
		{
			if (MyCharacter->GetCurrentInteractive()->MessageUI.Num() > 0)
			{
				// Called OnHide UI Event
				//InGameUI->OnShowUI();

				//InGameUI->OnAnimationShowMessageUI();

				//UITypewriterEffectComponent->InitialiseMessage(MyCharacter->GetCurrentInteractive()->MessageUI[0], 1.0f);

				bUIActive = true;
			}
		}

		// Timer to call interaction on the character
		GetWorld()->GetTimerManager().SetTimer(WaitToCallInteractTimerHandle, this, &AUntitledLittleThiefPlayerController::WaitToCallInteract, WaitToCallInteraction, false);
	}
	else
	{
		//ShowNextUIText();		
	}
	*/
}

//void AUntitledLittleThiefPlayerController::WaitToCallInteract()
//{
	//GetWorld()->GetTimerManager().ClearTimer(WaitToCallInteractTimerHandle);

	//MyCharacter->HandleInteract();

	// Call timer to unlock input and hide the UI
	//GetWorld()->GetTimerManager().SetTimer(FinishInteractTimerHandle, this, &AUntitledLittleThiefPlayerController::FinishInteracting, TimeInteracting, false);
//}

//void AUntitledLittleThiefPlayerController::FinishInteracting()
//{
	//GetWorld()->GetTimerManager().ClearTimer(FinishInteractTimerHandle);

	//bIsInputLocked = false;

	//if (bUIActive)
	//{
		//bUIActive = false;
		// Called OnHide UI Event
		//InGameUI->OnHideUI();
	//}
//}



void AUntitledLittleThiefPlayerController::ZoomIn()
{
	if (bIsInputLocked) return;

	if (MyCharacter != nullptr)
	{
		MyCharacter->ZoomIn();
	}
}

void AUntitledLittleThiefPlayerController::ZoomOut()
{
	if (bIsInputLocked) return;

	if (MyCharacter != nullptr)
	{
		MyCharacter->ZoomOut();
	}
}


////////////////// UI ///////////////////////////////

void AUntitledLittleThiefPlayerController::EnableUI()
{
	if (InGameUI == nullptr) return;

	bUIActive = true;

	// Call event
	//InGameUI->OnShowUI();

	//InGameUI->OnShowMessageUI();
}

void AUntitledLittleThiefPlayerController::DisableUI()
{
	if (InGameUI == nullptr) return;

	bUIActive = false;

	//InGameUI->OnHideUI();

	//InGameUI->OnHideMessageUI();
}



////////////////// UI ///////////////////////////////