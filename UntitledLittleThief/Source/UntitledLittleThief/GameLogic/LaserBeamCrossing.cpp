// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBeamCrossing.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UntitledLittleThiefCharacter.h"


ALaserBeamCrossing::ALaserBeamCrossing()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	BeamSource = CreateDefaultSubobject<USceneComponent>(TEXT("BeamSource")); 
	BeamSource->SetupAttachment(RootComponent);

	TargetSource = CreateDefaultSubobject<USceneComponent>(TEXT("TargetSource"));
	TargetSource->SetupAttachment(RootComponent);

	BeamCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BeamCollision"));
	BeamCollision->SetupAttachment(RootComponent);

	BeamCollision->OnComponentBeginOverlap.AddDynamic(this, &ALaserBeamCrossing::BeginOverlap);
	BeamCollision->OnComponentEndOverlap.AddDynamic(this, &ALaserBeamCrossing::EndOverlap);
}

void ALaserBeamCrossing::BeginPlay()
{
	Super::BeginPlay();
	
	if (BeamParticleSystem != nullptr)
	{
		BeamParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticleSystem, GetActorLocation());

		if (BeamParticleSystemComponent != nullptr)
		{
			// Set Beam Source
			BeamParticleSystemComponent->SetBeamSourcePoint(0, BeamSource->GetComponentLocation(), 0);
			LocalBeamSourcePointLocation = GetTransform().InverseTransformPosition(BeamSource->GetComponentLocation());


			// Set Target
			BeamParticleSystemComponent->SetBeamTargetPoint(0, TargetSource->GetComponentLocation(), 0);
			CurrentTargetPoint = TargetSource->GetComponentLocation();

			LocalTargetLocation = GetTransform().InverseTransformPosition(TargetSource->GetComponentLocation());		
		}
	}
}


void ALaserBeamCrossing::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if ((OtherActor != nullptr) && (BeamParticleSystemComponent != nullptr))
	{		
		// Transform to local the other actor location
		FVector OtherActorLocalLocation = GetTransform().InverseTransformPosition(OtherActor->GetActorLocation());	

		// Check that this actor is in the middle of both points
		if ((OtherActorLocalLocation.X >= LocalBeamSourcePointLocation.X) && (OtherActorLocalLocation.X <= LocalTargetLocation.X))
		{
			

		    AUntitledLittleThiefCharacter * Character = Cast<AUntitledLittleThiefCharacter>(OtherActor);

			if (Character)
			{
				Character->ChangeStatus(EStatus::VE_FALLING);
			}
			else
			{
				float DistanceToSource = LocalTargetLocation.X - OtherActorLocalLocation.X;

				FVector CurrentTarget = TargetSource->GetComponentLocation();

				CurrentTarget = CurrentTarget - (TargetSource->GetForwardVector() * DistanceToSource);

				LocalTargetLocation = GetTransform().InverseTransformPosition(CurrentTarget);

				BeamParticleSystemComponent->SetBeamTargetPoint(0, CurrentTarget, 0);
			}
		}
		
	}
}


void ALaserBeamCrossing::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (BeamParticleSystemComponent != nullptr))
	{
		FVector OtherActorLocalLocation = GetTransform().InverseTransformPosition(OtherActor->GetActorLocation());

		// Check that this actor is in the middle of both points
		if ((OtherActorLocalLocation.X >= LocalBeamSourcePointLocation.X) && (OtherActorLocalLocation.X <= LocalTargetLocation.X))
		{
			CurrentTargetPoint = TargetSource->GetComponentLocation();
			BeamParticleSystemComponent->SetBeamTargetPoint(0, CurrentTargetPoint, 0);
		}		
	}
}



