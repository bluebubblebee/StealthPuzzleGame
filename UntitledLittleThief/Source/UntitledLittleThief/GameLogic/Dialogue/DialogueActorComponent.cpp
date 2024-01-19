// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueActorComponent.h"
#include "DialogNPCHelper.h"
#include "Components/AudioComponent.h"
#include "UI/PlayerInGameUI.h"
#include "UntitledLittleThiefPlayerController.h"

UDialogueActorComponent::UDialogueActorComponent()
{
	AudioDialogue = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioDialog"));

	PrimaryComponentTick.bCanEverTick = true;
}


void UDialogueActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueActorComponent::Initialise(class AUntitledLittleThiefPlayerController* PC)
{
	CurrentPlayer = PC;

	State = EDialogueState::VE_NONE;

	// Find the first row
	CurrentDialogueRow = DialogueTable->FindRow<FDialogNPC>(FirstDialogueID, FString(""), true);

	if (CurrentDialogueRow == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueActorComponent::Initialise Unable to start dialogue system, No row found "));

		return;
	}

	if ((CurrentPlayer != nullptr) && (CurrentPlayer->GetUI() != nullptr))
	{
		State = EDialogueState::VE_PREPARING;


		InGameUI = CurrentPlayer->GetUI();
		InGameUI->SetMessage("");

		// Lock input and enable UI
		CurrentPlayer->LockInput();
		CurrentPlayer->EnableUI();

		// Initialize Character
		InGameUI->SetCharacterName("");
		InGameUI->SetMessage("");
		InGameUI->ResetAllOption();

		InGameUI->OnHideOptions();

		// Show UI Animation
		InGameUI->OnAnimationShowMessageUI();
		GetWorld()->GetTimerManager().SetTimer(InitialDelayTimerHandle, this, &UDialogueActorComponent::OnStartUIEffect, 1.0f, false);
	}

}

void UDialogueActorComponent::Start(class AUntitledLittleThiefPlayerController* PC, FName LineID)
{
	FirstDialogueID = LineID;

	Initialise(PC);
}


void UDialogueActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	if (State == EDialogueState::VE_ANIMATINGTEXT)
	{
		CurrentDeltaTime += DeltaTime;
		if (CurrentDeltaTime >= DelayBetweenWords)
		{
			if (WordIndex < WordList.Num())
			{
				CurrentDeltaTime = 0.0f;

				CurrentMessage += WordList[WordIndex] + " ";

				WordIndex += 1;

				InGameUI->SetMessage(CurrentMessage);

				if (SoundCueMessages.Num() > 0)
				{
					int32 RandIndexAudio = FMath::RandRange(0, SoundCueMessages.Num() - 1);
					AudioDialogue->SetSound(SoundCueMessages[RandIndexAudio]);
					AudioDialogue->Play();
				}
			}
			else
			{

				InGameUI->SetMessage(MessageToShow);

				State = EDialogueState::VE_WAITINPUTPLAYER; 
			}
		}

	}
}

void UDialogueActorComponent::StartTypeWriterEffect(const FString& Text)
{
	if (Text.Len() == 0) return;

	// Split by the messages in words
	Text.ParseIntoArray(WordList, TEXT(" "), true);

	WordIndex = 0;

	MessageToShow = Text;

	CurrentMessage = "";

	CurrentDeltaTime = 0.0f;

	InGameUI->SetMessage(CurrentMessage);

	State = EDialogueState::VE_ANIMATINGTEXT;
}


void UDialogueActorComponent::OnStartUIEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(InitialDelayTimerHandle);	

	// First time always stars with DialogText	   	
	if (CurrentDialogueRow->DialogText != "None") // Check first line
	{
		InGameUI->SetCharacterName(CurrentDialogueRow->CharacterName.ToString());
		InGameUI->SetMessage(CurrentDialogueRow->DialogText.ToString());

		// Show UI Animation
		StartTypeWriterEffect(CurrentDialogueRow->DialogText.ToString());
	}	

	CurrentPlayer->OnInteract.AddUniqueDynamic(this, &UDialogueActorComponent::OnPlayerInteract);
	CurrentPlayer->OnNavigateUI.AddUniqueDynamic(this, &UDialogueActorComponent::OnPlayerNavigateUI);
	
}

void UDialogueActorComponent::OnCompleteDialogue()
{
	UE_LOG(LogTemp, Warning, TEXT("UDialogueActorComponent:: OnCompleteDialogue"));

	GetWorld()->GetTimerManager().ClearTimer(InitialDelayTimerHandle);

	InGameUI->OnHideUI();

	// Lock input and enable UI
	CurrentPlayer->UnLockInput();

	CurrentPlayer->DisableUI();

	State = EDialogueState::VE_NONE;

	OnDialogueCompletedEvent.Broadcast();
}

void UDialogueActorComponent::OnPlayerInteract()
{
	if (CurrentDialogueRow == nullptr) return;


	if (State == EDialogueState::VE_ANIMATINGTEXT)  // If text animating, wait for player again
	{		
		InGameUI->SetMessage(MessageToShow);

		State = EDialogueState::VE_WAITINPUTPLAYER;
	}
	
	else if (State == EDialogueState::VE_WAITINPUTPLAYER) // Wait for player to press interact
	{
		// Reset all options
		InGameUI->ResetAllOption();
		InGameUI->OnHideOptions();
		
		
		// If there are options in this row
		if (CurrentDialogueRow->HasOptions()) 
		{
			// Show options

			InGameUI->OnHideMessage();
			InGameUI->SetCharacterName("");
			InGameUI->SetMessage("");


			SelectedOption = 0;

			PlayerOptionsID = CurrentDialogueRow->GetOptions();

			if (PlayerOptionsID.Num() >= 1)
			{
				//CurrentDialogueRow->PlayerOptionsChain.ToString().ParseIntoArray(PlayerOptionsID, TEXT(";"), true);
				for (int i = 0; i < PlayerOptionsID.Num(); i++)
				{
					//UE_LOG(LogTemp, Warning, TEXT("UDialogActorComponent::GetPlayerOptions %s "), *PlayerOptionsID[i]);

					FDialogNPC* OptionRow = DialogueTable->FindRow<FDialogNPC>(FName(*PlayerOptionsID[i]), FString(""), true);
					if (OptionRow != nullptr)
					{
						UE_LOG(LogTemp, Warning, TEXT("UDialogActorComponent::GetPlayerOptions %s %s "), *PlayerOptionsID[i] ,*OptionRow->DialogText.ToString());

						InGameUI->SetOption("> " + OptionRow->DialogText.ToString(), i);
						InGameUI->SetCharacterName(OptionRow->CharacterName.ToString());
					}				
				}

			//InGameUI->SetCharacterName(CurrentDialogueRow->CharacterName.ToString());

				InGameUI->OnShowOptions();			
				InGameUI->SelectOption(0);

				// Change wait to select  option
				State = EDialogueState::VE_WAITTOSELECTOPTION; // Wait to select option
			}
		}

		// If there is a next line to show on this row
		else if (CurrentDialogueRow->HasNextLine())
		{
			// Retrieve that line and start typewriter effect
			FDialogNPC* NextLine = DialogueTable->FindRow<FDialogNPC>(CurrentDialogueRow->NextLineID, FString(""), true);
			
			if (NextLine != nullptr)
			{
				CurrentDialogueRow = NextLine;

				InGameUI->SetCharacterName(CurrentDialogueRow->CharacterName.ToString());
				InGameUI->SetMessage(CurrentDialogueRow->DialogText.ToString());

				StartTypeWriterEffect(CurrentDialogueRow->DialogText.ToString());
			}
			else // No line found, complete dialogue
			{			

				InGameUI->OnAnimationHideMessageUI();
				GetWorld()->GetTimerManager().SetTimer(InitialDelayTimerHandle, this, &UDialogueActorComponent::OnCompleteDialogue, 1.0f, false);

				State = EDialogueState::VE_COMPLETE;
			}


		}
		else // No line found, complete dialogue
		{
			InGameUI->OnAnimationHideMessageUI();
			GetWorld()->GetTimerManager().SetTimer(InitialDelayTimerHandle, this, &UDialogueActorComponent::OnCompleteDialogue, 1.0f, false);
			State = EDialogueState::VE_COMPLETE;
		}

		


	}else if (State == EDialogueState::VE_WAITTOSELECTOPTION) // Wait to select an option
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueActorComponent:: OnPlayerInteract - VE_WAITTOSELECTOPTION: SelectedOption %d / %d"), SelectedOption, PlayerOptionsID.Num());


		if (SelectedOption < PlayerOptionsID.Num())
		{
			// Reset all options
			InGameUI->ResetAllOption();
			InGameUI->OnHideOptions();

			// Change dialogue line
			CurrentDialogueRow = DialogueTable->FindRow<FDialogNPC>(FName(*PlayerOptionsID[SelectedOption]), FString(""), true);

			// PlayerResponseID FOR THIS CurrentDialogueRow
			if ((CurrentDialogueRow != nullptr) && (CurrentDialogueRow->HasPlayerResponse()))
			{
				InGameUI->OnShowMessage();

				InGameUI->SetCharacterName("");
				InGameUI->SetMessage("");

				FDialogNPC* NPCLine = DialogueTable->FindRow<FDialogNPC>(CurrentDialogueRow->PlayerResponseID, FString(""), true);
				if (NPCLine != nullptr)
				{
					CurrentDialogueRow = NPCLine;
					if (CurrentDialogueRow != nullptr)
					{
						InGameUI->SetCharacterName(CurrentDialogueRow->CharacterName.ToString());
						InGameUI->SetMessage(CurrentDialogueRow->DialogText.ToString());

						State = EDialogueState::VE_ANIMATINGTEXT;

						StartTypeWriterEffect(CurrentDialogueRow->DialogText.ToString());
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UDialogueActorComponent:: OnPlayerInteract - VE_WAITTOSELECTOPTION: SelectedOption %d / %d"), SelectedOption, PlayerOptionsID.Num());


				// Show UI Animation
				InGameUI->OnAnimationHideMessageUI();
				GetWorld()->GetTimerManager().SetTimer(InitialDelayTimerHandle, this, &UDialogueActorComponent::OnCompleteDialogue, 1.0f, false);

				State = EDialogueState::VE_COMPLETE;
			}
		}	
	}
}

void UDialogueActorComponent::OnPlayerNavigateUI(const EInputDirection& Direction)
{
	if (InGameUI == nullptr) return;

	if (State != EDialogueState::VE_WAITTOSELECTOPTION) return;

	if (PlayerOptionsID.Num() == 1) return;		

	int NewOption = SelectedOption;


	switch (Direction)
	{
		case EInputDirection::VE_UP:			

			NewOption = SelectedOption - 1;

			if (NewOption < 0)
			{
				NewOption = PlayerOptionsID.Num()-1;

			}

			InGameUI->SelectOption(NewOption);
			InGameUI->DeselectOption(SelectedOption);

			SelectedOption = NewOption;
		break;

		case EInputDirection::VE_DOWN:

			NewOption = SelectedOption + 1;

			if (NewOption >= PlayerOptionsID.Num())
			{
				NewOption = 0;
			}

			InGameUI->SelectOption(NewOption);
			InGameUI->DeselectOption(SelectedOption);

			SelectedOption = NewOption;

		break;
	}
}



