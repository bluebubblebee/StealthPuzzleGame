// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInGameUI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(F_MESSAGE_COMPLETED_DELEGATE);

/**
 * 
 */
UCLASS()
class UNTITLEDLITTLETHIEF_API UPlayerInGameUI : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual bool Initialize() override;

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerDialogTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterNameText;

// Player Options
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OptionText_0;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OptionText_1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OptionText_2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OptionText_3;



public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnShowUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnHideUI();

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnAnimationShowMessageUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnAnimationHideMessageUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnShowMessage();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnHideMessage();


public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnAnimationShowOptions();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnAnimationHideOptions();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnShowOptions();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
	void OnHideOptions();

public:


	void SetMessage(const FString& Text);

	void SetCharacterName(const FString& Text);


public:
	void ResetAllOption();

	void SetOption(const FString& Text, int Option);

	void SelectOption(int Option);

	void DeselectOption(int Option);

};
