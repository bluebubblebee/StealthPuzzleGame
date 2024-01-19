// Fill out your copyright notice in the Description page of Project Settings.


#include "InitializeLevelTrigger.h"
#include "GameLogic/Dialogue/DialogueActorComponent.h"
#include "UntitledLittleThiefPlayerController.h"



AInitializeLevelTrigger::AInitializeLevelTrigger()
{

	DialogueSystem = CreateDefaultSubobject<UDialogueActorComponent>(TEXT("Dialogue"));

	PrimaryActorTick.bCanEverTick = false;
}


void AInitializeLevelTrigger::BeginPlay()
{
	Super::BeginPlay();	
}


void AInitializeLevelTrigger::InitializeLevel()
{
	CurrentPlayerController =  Cast<AUntitledLittleThiefPlayerController>(GetWorld()->GetFirstPlayerController());

	if (CurrentPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Initialize Dialogue System"));

		DialogueSystem->Initialise(CurrentPlayerController);		
	}
}


