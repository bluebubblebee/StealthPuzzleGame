// Fill out your copyright notice in the Description page of Project Settings.


#include "PushObject.h"
#include "UntitledLittleThiefCharacter.h"
#include "Components/BoxComponent.h"


void APushObject::Push(FVector Direction, float Force, float DeltaTime)
{
	if (!bIsEnabled) return;	

	FHitResult OutHit;
	FVector Start = GetActorLocation();
	Start.Z += 200.0f;

	FVector End = ((Direction * 100.f) + Start);
	FCollisionQueryParams CollisionParams;

	// Check collision with the world
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		if (OutHit.GetActor() != nullptr)
		{
			if (CurrentCharacter != nullptr)
			{
				CurrentCharacter->ClearInteractiveInRange(this);
			}
		}		
	}
	else
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation += (Force * Direction * DeltaTime);
		SetActorLocation(CurrentLocation);
	}
}


