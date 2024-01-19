// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Engine/DataTable.h"

#include "DialogNPCHelper.generated.h"

USTRUCT(BlueprintType)
struct FDialogNPC : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName; // Name of the character to display

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FromLine; // Helper to know where it's comming the previous line

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DialogText; // Text

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PlayerOptionsChain; // Options to display, separate by ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextLineID;  // Next line just in case there is need to break the dialog into different options	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PlayerResponseID; // Can be emptyThis field refers to the line to show after the player has selected a option PlayerOptionsChain

	bool HasOptions()
	{
		if (PlayerOptionsChain == "None") return false;

		return true;
	}

	bool HasNextLine()
	{
		if (NextLineID == "None") return false;

		return true;
	}

	bool HasPlayerResponse()
	{
		if (PlayerResponseID == "None") return false;

		return true;
	}

	TArray<FString> GetOptions()
	{
		TArray<FString> Options;
		PlayerOptionsChain.ToString().ParseIntoArray(Options, TEXT(";"), true);

		return Options;
	}

};

static UDataTable* DialogNPCTable;

static const FString ContextString(TEXT("Dialog"));

/**
 * 
 */
UCLASS()
class UNTITLEDLITTLETHIEF_API UDialogNPCHelper : public UObject
{
	GENERATED_BODY()

public:
	UDialogNPCHelper(const FObjectInitializer& ObjectInitializer);

	static FDialogNPC* GetLineDialog(const FName& ID);

	
};
