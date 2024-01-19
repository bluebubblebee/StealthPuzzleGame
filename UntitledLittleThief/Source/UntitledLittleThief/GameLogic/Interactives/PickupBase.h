// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Definitions.h"
#include "PickupBase.generated.h"

UCLASS()
class UNTITLEDLITTLETHIEF_API APickupBase : public AActor
{
	GENERATED_BODY()

protected:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	class UStaticMeshComponent* PickupMesh;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	EPickupType PickupType;
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

	FORCEINLINE EPickupType GetPickupType() const {return PickupType;}


};
