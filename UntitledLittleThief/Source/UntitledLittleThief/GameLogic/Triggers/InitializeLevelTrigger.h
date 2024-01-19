// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Definitions.h"
#include "InitializeLevelTrigger.generated.h"

UCLASS()
class UNTITLEDLITTLETHIEF_API AInitializeLevelTrigger : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelTrigger")
	class UDialogueActorComponent* DialogueSystem;
	
public:	
	AInitializeLevelTrigger();

	UFUNCTION(BlueprintCallable, Category = "Level Events ")
	void InitializeLevel();	

protected:
	virtual void BeginPlay() override;
protected:
	class AUntitledLittleThiefPlayerController* CurrentPlayerController;

};
