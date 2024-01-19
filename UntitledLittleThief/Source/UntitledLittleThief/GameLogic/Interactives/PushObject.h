// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveBase.h"
#include "Utils/Definitions.h"
#include "PushObject.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDLITTLETHIEF_API APushObject : public AInteractiveBase
{
	GENERATED_BODY()

public:

	void Push(FVector Direction, float Force, float DeltaTime);
};
