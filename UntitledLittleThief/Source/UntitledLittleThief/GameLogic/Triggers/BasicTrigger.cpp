// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTrigger.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "UntitledLittleThiefCharacter.h"
#include "UntitledLittleThiefPlayerController.h"


ABasicTrigger::ABasicTrigger()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractiveMesh"));
	Mesh->SetupAttachment(RootComponent);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);


	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABasicTrigger::BeginOverlap);

	PrimaryActorTick.bCanEverTick = false;

}

void ABasicTrigger::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!bEnableTrigger) return;

	if (OtherActor == nullptr) return;

	AUntitledLittleThiefCharacter* MainCharacter = Cast<AUntitledLittleThiefCharacter>(OtherActor);

	if (MainCharacter)
	{
		CurrentPlayerController = Cast<AUntitledLittleThiefPlayerController>(MainCharacter->GetController());		
	}
}


void ABasicTrigger::BeginPlay()
{
	Super::BeginPlay();

	bEnableTrigger = true;
}

void ABasicTrigger::DisableTrigger()
{
	bEnableTrigger = false;

	Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Stop collision

	Trigger->SetGenerateOverlapEvents(false);
}



