// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interactives/InteractiveBase.h"
#include "PressurePad.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDLITTLETHIEF_API APressurePad : public AInteractiveBase
{
	GENERATED_BODY()

public:
	APressurePad();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pressure Pad")
	class UDialogueActorComponent* DialogueSystem;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Pressure Pad")
	TSubclassOf<class ABridgeTile> TileToSpawnClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pressure Pad")
	int32 TileNumberToSpawn = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Pressure Pad")
	float DistanceXBetweenTiles = 300;

	UPROPERTY(EditInstanceOnly, Category = "Pressure Pad")
	AActor* TileSpawnPoint;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Pressure Pad")
	EPickupType PressureObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pressure Pad")
	FName DialogueIDObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pressure Pad")
	FName DialogueIDNoObject;

public:

	virtual void Tick(float DeltaTime) override;

	/////// Interactions /////////

	virtual void StartInteract(APawn* PawnInstigator) override;

	/////// Interactions /////////

protected:

	virtual void BeginPlay() override;

	// Blueprint Event to open door
	UFUNCTION(BlueprintImplementableEvent, Category = "Pressured Pad")
	void OnPressedPad();	

	UFUNCTION()
	void OnBridgeTileEffectCompleted();

	void ShowBridge();

	void HideBridge();

protected:

	UFUNCTION()
	void OnDialogueComplete();
	
protected:
	
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	   

private:

	TArray<ABridgeTile*> TileList;

	int32 NumberTilesEffectCompleted;

	class AUntitledLittleThiefPlayerController* CurrentController;
};
