// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInGameUI.h"
#include "Components/TextBlock.h"
#include "UI/TypewriterEffectComponent.h"


bool UPlayerInGameUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	return true;
}

void UPlayerInGameUI::SetMessage(const FString& Text)
{
	if (PlayerDialogTextBlock == nullptr) return;

	PlayerDialogTextBlock->SetText(FText::FromString(Text));
}


void UPlayerInGameUI::SetCharacterName(const FString& Text)
{
	if (CharacterNameText == nullptr) return;

	CharacterNameText->SetText(FText::FromString(Text));
}

void UPlayerInGameUI::SetOption(const FString& Text, int Option)
{
	if ((OptionText_0 == nullptr) || (OptionText_1 == nullptr) || (OptionText_2 == nullptr) || (OptionText_3 == nullptr)) return;

	switch (Option)
	{
		case 0:
			OptionText_0->SetText(FText::FromString(Text));

		break;
		case 1:
			OptionText_1->SetText(FText::FromString(Text));

			break;
		case 2:
			OptionText_2->SetText(FText::FromString(Text));

		break;

		case 3:
			OptionText_3->SetText(FText::FromString(Text));

		break;
	}
	
}

void UPlayerInGameUI::SelectOption(int Option)
{
	if ((OptionText_0 == nullptr) || (OptionText_1 == nullptr) || (OptionText_2 == nullptr) || (OptionText_3 == nullptr)) return;

	FSlateColor SelectColor = FSlateColor(FLinearColor(0.0f, 0.6f, 1.0f, 1.0f));

	switch (Option)
	{
	case 0:
		OptionText_0->SetColorAndOpacity(SelectColor);

		break;
	case 1:
		OptionText_1->SetColorAndOpacity(SelectColor);

		break;
	case 2:
		OptionText_2->SetColorAndOpacity(SelectColor);

		break;

	case 3:
		OptionText_3->SetColorAndOpacity(SelectColor);

		break;
	}
}

void UPlayerInGameUI::DeselectOption(int Option)
{
	if ((OptionText_0 == nullptr) || (OptionText_1 == nullptr) || (OptionText_2 == nullptr) || (OptionText_3 == nullptr)) return;

	FSlateColor DeselectColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	switch (Option)
	{
	case 0:
		OptionText_0->SetColorAndOpacity(DeselectColor);

		break;
	case 1:
		OptionText_1->SetColorAndOpacity(DeselectColor);

		break;
	case 2:
		OptionText_2->SetColorAndOpacity(DeselectColor);

		break;

	case 3:
		OptionText_3->SetColorAndOpacity(DeselectColor);

		break;
	}
}

void UPlayerInGameUI::ResetAllOption()
{
	SetOption("", 0);
	SetOption("", 1);
	SetOption("", 2);
	SetOption("", 3);

	DeselectOption(0);
	DeselectOption(1);
	DeselectOption(2);
	DeselectOption(3);
}