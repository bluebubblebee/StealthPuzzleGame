// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UntitledLittleThiefCharacter.h"



AInteractiveBase::AInteractiveBase()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	RootComponent = RootScene;

	// Mesh
	InteractiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractiveMesh"));
	InteractiveMesh->SetupAttachment(RootComponent);

	LookAtPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LookAtPoint"));
	LookAtPoint->SetupAttachment(RootComponent);

	InteractiveCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractiveCollision")); 
	InteractiveCollision->SetupAttachment(RootComponent);

	InteractiveCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveBase::BeginOverlap);
	InteractiveCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractiveBase::EndOverlap);

	PrimaryActorTick.bCanEverTick = true;

}

void AInteractiveBase::BeginPlay()
{
	Super::BeginPlay();

	bIsEnabled = true;
	
}

void AInteractiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AInteractiveBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!bIsEnabled) return;

	CurrentCharacter = Cast<AUntitledLittleThiefCharacter>(OtherActor);

	if (CurrentCharacter)
	{
		CurrentCharacter->SetInteractiveInRange(this);
	}
}

void AInteractiveBase::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIsEnabled) return;

	CurrentCharacter = Cast<AUntitledLittleThiefCharacter>(OtherActor);

	if (CurrentCharacter)
	{
		CurrentCharacter->ClearInteractiveInRange(this);
	}
}

void AInteractiveBase::SetEnable(bool Enable)
{
	bIsEnabled = Enable;

	if (bIsEnabled)
	{
		// Only used for spatial queries (raycasts, sweeps, and overlaps).
		InteractiveCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		//Remove collision completely
		InteractiveCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}



void AInteractiveBase::StartInteract(APawn* PawnInstigator)
{
	if (!bIsEnabled) return;
	
}

void AInteractiveBase::EndInteract(APawn* PawnInstigator)
{
	if (!bIsEnabled) return;

}
