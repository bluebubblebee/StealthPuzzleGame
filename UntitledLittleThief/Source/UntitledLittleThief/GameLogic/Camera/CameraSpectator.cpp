// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSpectator.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACameraSpectator::ACameraSpectator()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractiveMesh"));
	Mesh->SetupAttachment(RootComponent);


	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraSpectator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraSpectator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

