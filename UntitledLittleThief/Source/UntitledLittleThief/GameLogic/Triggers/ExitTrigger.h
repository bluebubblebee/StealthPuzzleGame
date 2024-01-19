// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic/Triggers/BasicTrigger.h"
#include "ExitTrigger.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDLITTLETHIEF_API AExitTrigger : public ABasicTrigger
{
	GENERATED_BODY()

protected:

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit Trigger Settings")
	FName NextLevelName;


protected:
	FTimerHandle DelayToExitLevel;

	UFUNCTION()
	void OnExitLevel();
	
};
