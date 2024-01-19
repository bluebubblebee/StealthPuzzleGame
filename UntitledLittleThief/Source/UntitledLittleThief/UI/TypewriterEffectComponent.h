// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TypewriterEffectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(F_MESSAGE_COMPLETED);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDLITTLETHIEF_API UTypewriterEffectComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Message Settings")
	class UAudioComponent* AudioDialog;

public:	

	UTypewriterEffectComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;



protected:
	UPROPERTY(EditAnywhere, Category = "UI Message Settings")
	float DelayBetweenLetters = 0.5f;

	UPROPERTY(EditAnywhere, Category = "UI Message Settings")
	float DelayBetweenWords = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Message Settings")
	TArray<class USoundBase*> SoundCueMessages;

public:	

	UFUNCTION(BlueprintCallable, Category = "UI Messages Settings")
	void Initialise(class UPlayerInGameUI* UI);

	void InitialiseMessage(const FString& Text, float Delay = 0.0f);

	void ShowCompleteCurrentMessage();

	F_MESSAGE_COMPLETED OnMessageCompletedEvent;	

	FORCEINLINE bool IsMessageUpdating() { return bUpdateMessage; }

private: 

	float InitialDelay;

	bool bWaitToShowMessage;

	class UPlayerInGameUI* InGameUI;

	TArray<FString> WordList;

	int32 WordIndex;

	bool bUpdateMessage;

	FString CurrentMessage;	

	FString MessageToShow;

	float CurrentDeltaTime = 0.0f;		
};
