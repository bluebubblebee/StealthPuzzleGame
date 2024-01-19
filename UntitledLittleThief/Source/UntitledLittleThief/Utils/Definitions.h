// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType) // Animation type for the character
enum class EAnimationType : uint8
{
	VE_NONE 		    UMETA(DisplayName = "None"),
	VE_IDLE				UMETA(DisplayName = "Idle"),
	VE_DISMISS		    UMETA(DisplayName = "Dismiss"),
	VE_INTERACT		    UMETA(DisplayName = "Interact"),
	VE_PUSH             UMETA(DisplayName = "Push"),
	VE_FALL             UMETA(DisplayName = "Fall"),
};


UENUM(BlueprintType) // Statuse for the character
enum class EStatus : uint8
{
	VE_NONE 		    UMETA(DisplayName = "None"),
	VE_PUSHING          UMETA(DisplayName = "Pushing"),
	VE_PICKING_ITEM     UMETA(DisplayName = "PickingItem"),
	VE_FALLING          UMETA(DisplayName = "Falling"),
	VE_CARRYING_ITEM    UMETA(DisplayName = "Carrying Item"),
};


UENUM(BlueprintType) // Push direction
enum class EPushDirection : uint8
{
	VE_NONE 		    UMETA(DisplayName = "None"),
	VE_TO_TOP           UMETA(DisplayName = "ToTop"),
	VE_TO_DOWN          UMETA(DisplayName = "ToDown"),
	VE_TO_LEFT          UMETA(DisplayName = "ToLeft"),
	VE_TO_RIGHT         UMETA(DisplayName = "ToRight"),
};

UENUM(BlueprintType) // Animation type for the character
enum class EInputDirection : uint8
{
	VE_NONE 		  UMETA(DisplayName = "None"),
	VE_UP             UMETA(DisplayName = "Up"),
	VE_RIGHT          UMETA(DisplayName = "Right"),
	VE_DOWN           UMETA(DisplayName = "Down"),
	VE_LEFT           UMETA(DisplayName = "Left"),
};

UENUM(BlueprintType) // Animation type for the character
enum class EPickupType : uint8
{
	VE_NONE 		  UMETA(DisplayName = "None"),
	VE_KEYDOOR        UMETA(DisplayName = "Key Door"),
	VE_PRESSUREBOX    UMETA(DisplayName = "Pressure Box")
};


// GetEnumValueAsString<EAnimationType>("EAnimationType", AnimationEnum)));
template<typename TEnum>
static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameByValue((int64)Value).ToString();
};

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	return (EnumType)Enum->FindEnumIndex(FName(*String));
};

class UNTITLEDLITTLETHIEF_API Definitions
{
public:
	Definitions();
	~Definitions();
};
