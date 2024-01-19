


#include "ExitTrigger.h"
#include "UntitledLittleThiefPlayerController.h"
#include "Kismet/GameplayStatics.h"



void AExitTrigger::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (CurrentPlayerController != nullptr)
	{
		CurrentPlayerController->LockInput();
	}

	// Disable trigger completely
	DisableTrigger();

	GetWorld()->GetTimerManager().SetTimer(DelayToExitLevel, this, &AExitTrigger::OnExitLevel, 1.0f, false);
	   	
}

void AExitTrigger::OnExitLevel()
{

	GetWorld()->GetTimerManager().ClearTimer(DelayToExitLevel);

	UGameplayStatics::OpenLevel(this, NextLevelName);
}