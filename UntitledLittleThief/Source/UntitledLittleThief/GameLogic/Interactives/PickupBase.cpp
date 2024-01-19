// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"


APickupBase::APickupBase()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true; 

}


