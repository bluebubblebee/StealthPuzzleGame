// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerCameraSpectator.h"
#include "UntitledLittleThiefCharacter.h"
#include "UntitledLittleThiefPlayerController.h"
#include "GameLogic/Dialogue/DialogueActorComponent.h"



ATriggerCameraSpectator::ATriggerCameraSpectator()
{	
	DialogueSystem = CreateDefaultSubobject<UDialogueActorComponent>(TEXT("Dialogue"));

	PrimaryActorTick.bCanEverTick = false;

}

void ATriggerCameraSpectator::BeginPlay()
{
	Super::BeginPlay();
}

void ATriggerCameraSpectator::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!bEnableTrigger) return;

	if ((CameraSpectator == nullptr) || (CurrentPlayerController == nullptr)) return;
	
	// Disable trigger completely
	DisableTrigger();
	
	// Start camera transtion
	CurrentPlayerController->SetViewTargetWithBlend(CameraSpectator, BlendCameraTransitionTime, EViewTargetBlendFunction::VTBlend_Cubic, false);

	GetWorld()->GetTimerManager().SetTimer(DelayToStartTransitionTimerHandle, this, &ATriggerCameraSpectator::OnDelayToStartTransition, DelayToStartTransition, false);
	
}

void ATriggerCameraSpectator::OnDelayToStartTransition()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayToStartTransitionTimerHandle);		

	DialogueSystem->Initialise(CurrentPlayerController);
	DialogueSystem->OnDialogueCompletedEvent.AddUniqueDynamic(this, &ATriggerCameraSpectator::OnDialogueComplete);
}

void ATriggerCameraSpectator::OnDialogueComplete()
{

	GetWorld()->GetTimerManager().SetTimer(DelayToStartTransitionTimerHandle, this, &ATriggerCameraSpectator::OnDelayToCompleted, 1.0f, false);

	// Go back to the initial
	CurrentPlayerController->SetViewTargetWithBlend(CurrentPlayerController->GetCharacter(), BlendCameraTransitionTime, EViewTargetBlendFunction::VTBlend_Cubic, false);	
}

void ATriggerCameraSpectator::OnDelayToCompleted()
{
	CurrentPlayerController->UnLockInput();
}

