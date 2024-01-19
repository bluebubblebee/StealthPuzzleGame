// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogNPCHelper.h"


UDialogNPCHelper::UDialogNPCHelper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	//static ConstructorHelpers::FObjectFinder<UDataTable> TableAsset(TEXT("/Game/Game/Data/Objects.Objects"));

	//if (TableAsset.Object)
	//{

		//DialogNPCTable = TableAsset.Object;
	//}

	
}


FDialogNPC* UDialogNPCHelper::GetLineDialog(const FName& ID)
{
	FDialogNPC* Row = DialogNPCTable->FindRow<FDialogNPC>(ID, ContextString);

	return Row;
}

