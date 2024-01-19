// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UntitledLittleThiefGameMode.h"
#include "UntitledLittleThiefPlayerController.h"
#include "UntitledLittleThiefCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUntitledLittleThiefGameMode::AUntitledLittleThiefGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUntitledLittleThiefPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Game/Blueprints/BP_RobotCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}