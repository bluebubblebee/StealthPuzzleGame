// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BridgeTile.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTileEffectCompleted);
UCLASS()
class UNTITLEDLITTLETHIEF_API ABridgeTile : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BridgeTitle")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BridgeTitle")
	class UStaticMeshComponent* Tile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BridgeTitle")
	class UBoxComponent* Collision;
	
public:	

	ABridgeTile();


	void EnableEffect(bool EffectUp, float StartDelay);

	void UpdateEffect(float DeltaTime);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FTileEffectCompleted OnEffectCompleted;
	

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnTileShown();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnTileHidden();

private:

	UMaterialInstanceDynamic* TileMaterial;

	bool bEnableEffect;

	bool bEffectGoUp;

	bool bWaitForDelay;
	float CurrentStartDelay;

	float CurrentDissolveEffect;
};
