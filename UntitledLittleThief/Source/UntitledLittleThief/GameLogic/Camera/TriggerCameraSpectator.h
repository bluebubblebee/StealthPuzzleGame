// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameLogic/Triggers/BasicTrigger.h"
#include "TriggerCameraSpectator.generated.h"

UCLASS()
class UNTITLEDLITTLETHIEF_API ATriggerCameraSpectator : public ABasicTrigger
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelTrigger")
	class UDialogueActorComponent* DialogueSystem;

	UFUNCTION()
	void OnDialogueComplete();

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera Trigger Settings")
	class AActor* CameraSpectator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Trigger  Settings")
	float BlendCameraTransitionTime = 0.7f;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Trigger  Settings")
	float DelayToStartTransition = 0.7f;

protected:

	FTimerHandle DelayToStartTransitionTimerHandle;

	UFUNCTION()
	void OnDelayToStartTransition();

	UFUNCTION()
	void OnDelayToCompleted();
	
public:	
	ATriggerCameraSpectator();

protected:

	virtual void BeginPlay() override;

protected:

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

};
