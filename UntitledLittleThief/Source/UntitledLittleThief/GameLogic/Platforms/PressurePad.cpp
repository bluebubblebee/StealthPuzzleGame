// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePad.h"
#include "BridgeTile.h"
#include "Components/BoxComponent.h"
#include "UntitledLittleThiefCharacter.h"
#include "UntitledLittleThiefPlayerController.h"
#include "GameLogic/Dialogue/DialogueActorComponent.h"

APressurePad::APressurePad()
{
	DialogueSystem = CreateDefaultSubobject<UDialogueActorComponent>(TEXT("DialogueSystem"));
}

void APressurePad::BeginPlay()
{
	Super::BeginPlay();

	// Disable Tick
	SetActorTickEnabled(false);

	if ((TileToSpawnClass == nullptr) || (TileSpawnPoint == nullptr)) return;

	FVector SpawnPosition = TileSpawnPoint->GetActorLocation();

	for (int iTile = 0; iTile < TileNumberToSpawn; ++iTile)
	{
		
		// Spawn each cell
		//FActorSpawnParameters SpawnParamenters;
		//SpawnParamenters.Owner = this;
		//SpawnParamenters.Instigator = Instigator;

		ABridgeTile* SpawnedTile = GetWorld()->SpawnActor<ABridgeTile>(TileToSpawnClass, SpawnPosition, FRotator::ZeroRotator);

		if (SpawnedTile != nullptr)
		{
			// Attach cell to this ator
			SpawnedTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			SpawnedTile->OnEffectCompleted.AddDynamic(this, &APressurePad::OnBridgeTileEffectCompleted);

			TileList.Add(SpawnedTile);

		}

		SpawnPosition.Y -= DistanceXBetweenTiles;		
	}
}

void APressurePad::Tick(float DeltaTime)
{	

	for (int iTile = 0; iTile < TileList.Num(); ++iTile)
	{
		TileList[iTile]->UpdateEffect(DeltaTime);
	}
}

void APressurePad::ShowBridge()
{
	// Add delay
	float DelayEffect = 0.0f;

	for (int iTile = 0; iTile < TileList.Num(); ++iTile)
	{
		TileList[iTile]->EnableEffect(true, DelayEffect);

		DelayEffect += 0.05f;
	}

	NumberTilesEffectCompleted = TileList.Num();

	SetActorTickEnabled(true);
}

void APressurePad::HideBridge()
{
	// Add delay
	float DelayEffect = 0.0f;

	for (int iTile = 0; iTile < TileList.Num(); ++iTile)
	{
		TileList[iTile]->EnableEffect(false, DelayEffect);

		DelayEffect += 0.05f;
	}

	NumberTilesEffectCompleted = TileList.Num();

	SetActorTickEnabled(true);

	OnPressedPad();
}

void APressurePad::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	
}

void APressurePad::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	if (CurrentCharacter == nullptr)  return;

	

}

void APressurePad::StartInteract(APawn* PawnInstigator)
{
	UE_LOG(LogTemp, Warning, TEXT("APressurePad::StartInteract"));

	if (CurrentCharacter == nullptr) return;

	CurrentController = Cast<AUntitledLittleThiefPlayerController>(CurrentCharacter->GetController());

	if (CurrentController == nullptr) return;
	
	DialogueSystem->OnDialogueCompletedEvent.AddUniqueDynamic(this, &APressurePad::OnDialogueComplete);

	if (CurrentCharacter->GetCurrentPickup() != nullptr)
	{
		if (CurrentCharacter->GetCurrentPickup()->GetPickupType() == EPickupType::VE_PRESSUREBOX) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has pickup  for pressure pad"));

			DialogueSystem->Start(CurrentController, DialogueIDObject);			

			OnPressedPad();

			ShowBridge();

			Super::StartInteract(PawnInstigator);

			// Disable the item
			SetEnable(false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has pickup but wrong type"));

			CurrentCharacter->Animate(EAnimationType::VE_DISMISS);

			CurrentController->LockInput();

			DialogueSystem->Start(CurrentController, DialogueIDNoObject);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player doesn't have a pressure pickup"));

		CurrentCharacter->Animate(EAnimationType::VE_DISMISS);


		CurrentController->LockInput();

		DialogueSystem->Start(CurrentController, DialogueIDNoObject);
	}
}


void APressurePad::OnBridgeTileEffectCompleted()
{
	NumberTilesEffectCompleted -= 1;	

	if (NumberTilesEffectCompleted <= 0)
	{
		SetActorTickEnabled(false);
	}
}


void APressurePad::OnDialogueComplete()
{
	if (CurrentController != nullptr)
	{
		CurrentController->UnLockInput();
	}
}