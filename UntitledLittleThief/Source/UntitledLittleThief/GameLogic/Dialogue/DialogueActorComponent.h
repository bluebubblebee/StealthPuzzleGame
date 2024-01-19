// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utils/Definitions.h"
#include "DialogNPCHelper.h"
#include "DialogueActorComponent.generated.h"


UENUM(BlueprintType) // Animation type for the character
enum class EDialogueState : uint8
{
	VE_NONE 							UMETA(DisplayName = "None"),
	VE_PREPARING						UMETA(DisplayName = "Preparing"),
	VE_ANIMATINGTEXT					UMETA(DisplayName = "AnimatingText"),
	VE_WAITINPUTPLAYER					UMETA(DisplayName = "WaitInputPlayer"), // Waits for player to press interact
	VE_WAITTOSELECTOPTION				UMETA(DisplayName = "WaitToSelectOption"), // Waits for player to select and option and press interact

	VE_COMPLETE							UMETA(DisplayName = "Complete")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDIALOGUE_COMPLETED);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDLITTLETHIEF_API UDialogueActorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue")
	class UDataTable* DialogueTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	class UAudioComponent* AudioDialogue;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelTrigger")
	FName FirstDialogueID;

protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Typewriter Effect Settings")
	float DelayBetweenWords = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Typewriter Effect Settings")
	TArray<class USoundBase*> SoundCueMessages;	


public:	

	UDialogueActorComponent();

	UFUNCTION(BlueprintCallable, Category = "UI Messages Settings")
	void Initialise(class AUntitledLittleThiefPlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "UI Messages Settings")
	void Start(class AUntitledLittleThiefPlayerController* PC, FName LineID);

	FDIALOGUE_COMPLETED OnDialogueCompletedEvent;

private:

	void StartTypeWriterEffect(const FString& Text);

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	class AUntitledLittleThiefPlayerController* CurrentPlayer;
	class UPlayerInGameUI* InGameUI;

private:
	FTimerHandle InitialDelayTimerHandle;

	UFUNCTION()
	void OnStartUIEffect();

	UFUNCTION()
	void OnCompleteDialogue();


private: // Player interaction
	UFUNCTION()
	void OnPlayerInteract();

	UFUNCTION()
	void OnPlayerNavigateUI(const EInputDirection& Direction);
	

private:	

	TArray<FString> WordList;

	int32 WordIndex;	

	FString CurrentMessage;

	FString MessageToShow;

	float CurrentDeltaTime = 0.0f;


private:
	EDialogueState State = EDialogueState::VE_NONE;
	FDialogNPC* CurrentDialogueRow;
	TArray<FString> PlayerOptionsID;
	int32 SelectedOption;		
};
