// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Utils/Definitions.h"
#include "UntitledLittleThiefPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(F_ONINTERACT_DELEGATE);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNAVIGATEUI, const EInputDirection&, Direction);

UCLASS()
class AUntitledLittleThiefPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUntitledLittleThiefPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void LockInput();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void UnLockInput();

protected:
	
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;
	// End PlayerController interface

protected:

	///// Input Actions //////

	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void EnableRunMode();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DisableRunMode();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void EnableStealthMode();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DisableStealthMode();

	UFUNCTION(BlueprintCallable, Category = "Inputs")
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void ZoomIn();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void ZoomOut(); 
	 
	///// Input Actions //////



	///// Interactions //////


	bool blockNavigateUI = false;
	float lockNavigateUIDeltaTime = 0.0f;

public: 

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FNAVIGATEUI OnNavigateUI;

	///// Interactions //////


protected:

	// Reference to the pawn
	class AUntitledLittleThiefCharacter* MyCharacter;

	bool bIsInputLocked;

	///// UI //////
protected:

	UPROPERTY(BlueprintReadWrite, Category = "Player UI")
	class UPlayerInGameUI* InGameUI;
	
	TSubclassOf<class UUserWidget> InGameUIClass;


public:

	FORCEINLINE UPlayerInGameUI* GetUI() { return InGameUI; };



	void EnableUI();

	void DisableUI();


	F_ONINTERACT_DELEGATE OnInteract;

private:

	bool bUIActive;



};


