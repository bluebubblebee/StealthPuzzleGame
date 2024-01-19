// Fill out your copyright notice in the Description page of Project Settings.


#include "BridgeTile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


ABridgeTile::ABridgeTile()
{

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Tile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	Tile->SetupAttachment(RootComponent);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BridgeTitle"));
	Collision->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = false;
}


void ABridgeTile::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInterface* Material = Tile->GetMaterial(0);
	TileMaterial = Tile->CreateDynamicMaterialInstance(0, Material);

	CurrentDissolveEffect = 0.0f;

	if (TileMaterial != nullptr)
	{
		TileMaterial->SetScalarParameterValue("DissolveValue", CurrentDissolveEffect);
	}	

	bEnableEffect = false;
}

void ABridgeTile::EnableEffect(bool EffectUp, float StartDelay)
{
	if (StartDelay > 0.0f)
	{
		CurrentStartDelay = StartDelay;
		bWaitForDelay = true;
	}
	else
	{
		CurrentStartDelay = 0.0f;
		bWaitForDelay = false;
	}

	bEnableEffect = true;
	bEffectGoUp = EffectUp;
}

void ABridgeTile::UpdateEffect(float DeltaTime)
{
	if (!bEnableEffect) return;

	if (bWaitForDelay)
	{
		CurrentStartDelay -= DeltaTime;

		if (CurrentStartDelay <= 0.0f)
		{
			CurrentStartDelay = 0.0f;
			bWaitForDelay = false;
		}
	}
	else
	{

		if (bEffectGoUp)
		{
			CurrentDissolveEffect += DeltaTime;

			TileMaterial->SetScalarParameterValue("DissolveValue", CurrentDissolveEffect);

			if (CurrentDissolveEffect >= 1.0f)
			{
				CurrentDissolveEffect = 1.0f;

				bEnableEffect = false;

				OnTileShown();

				OnEffectCompleted.Broadcast();
			}
		}
		else
		{
			CurrentDissolveEffect -= DeltaTime;

			TileMaterial->SetScalarParameterValue("DissolveValue", CurrentDissolveEffect);

			if (CurrentDissolveEffect <= 0.0f)
			{
				CurrentDissolveEffect = 0.0f;

				bEnableEffect = false;

				OnTileHidden();

				OnEffectCompleted.Broadcast();
			}
		}
	}
}



