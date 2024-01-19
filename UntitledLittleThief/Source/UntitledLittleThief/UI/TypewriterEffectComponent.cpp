// Fill out your copyright notice in the Description page of Project Settings.


#include "TypewriterEffectComponent.h"
#include "Components/AudioComponent.h"
#include "PlayerInGameUI.h"


UTypewriterEffectComponent::UTypewriterEffectComponent()
{

	AudioDialog = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioDialog"));
	//AudioDialog->SetupAttachment(Root);

	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UTypewriterEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTypewriterEffectComponent::Initialise(class UPlayerInGameUI* UI)
{
	InGameUI = UI;
}


void UTypewriterEffectComponent::InitialiseMessage(const FString& Text, float Delay)
{
	UE_LOG(LogTemp, Warning, TEXT("UTypewriterEffectComponent::InitialiseMessage Called "));

	if ((InGameUI == nullptr) || (Text.Len() == 0)) return;

	InitialDelay = Delay;

	// Split by the messages in words
	Text.ParseIntoArray(WordList, TEXT(" "), true);

	WordIndex = 0;

	MessageToShow = Text;

	CurrentMessage = "";

	CurrentDeltaTime = 0.0f;

	InGameUI->SetMessage(CurrentMessage);

	bWaitToShowMessage = true;

	bUpdateMessage = true;
}

void UTypewriterEffectComponent::ShowCompleteCurrentMessage()
{
	if (InGameUI == nullptr) return;

	bWaitToShowMessage = false;

	bUpdateMessage = false;

	InGameUI->SetMessage(MessageToShow);

	OnMessageCompletedEvent.Broadcast();
}



void UTypewriterEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if ((InGameUI == nullptr) || (!bUpdateMessage)) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentDeltaTime += DeltaTime;

	if (bWaitToShowMessage)
	{
		if (CurrentDeltaTime >= InitialDelay)
		{
			bWaitToShowMessage = false;
			CurrentDeltaTime = 0.0f;

		}
	}
	else
	{
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
					AudioDialog->SetSound(SoundCueMessages[RandIndexAudio]);
					AudioDialog->Play();
				}
			}
			else
			{
				bUpdateMessage = false;

				InGameUI->SetMessage(MessageToShow);

				OnMessageCompletedEvent.Broadcast();
			}
		}
	}

}

