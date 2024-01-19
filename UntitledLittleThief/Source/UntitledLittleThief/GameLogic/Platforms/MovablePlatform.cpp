

#include "MovablePlatform.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMovablePlatform::AMovablePlatform()
{
	// Create root
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	// Create Static Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;

}


void AMovablePlatform::BeginPlay()
{
	Super::BeginPlay();

	bIsWaiting = false;
	bIsMoving = false;
	bIsSwitchedOn = false;

	if ((StartPoint != nullptr) && (EndPoint != nullptr))
	{
		StartTargetLocation = StartPoint;
		TargetLocation = EndPoint;
		SetActorLocation(StartTargetLocation->GetActorLocation());

		Direction = TargetLocation->GetActorLocation() - StartTargetLocation->GetActorLocation();

		TotalDistance = Direction.Size();

		// Get Normal Direction
		Direction = Direction.GetSafeNormal();
		CurrentDistance = 0.0f;
	}
	
}


void AMovablePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving) return;

	if (CurrentDistance < TotalDistance)
	{
		FVector Location = GetActorLocation();

		Location += Direction * Speed * DeltaTime;

		SetActorLocation(Location);

		CurrentDistance = (Location - StartTargetLocation->GetActorLocation()).Size();
	}
	else
	{
		// Start timer
		bIsMoving = false;
		GetWorld()->GetTimerManager().SetTimer(TimerWaitOnTarget, this, &AMovablePlatform::OnTimerWaitOnTargetCompleted, WaitTimeOnTarget, false);
		
	}
}

void AMovablePlatform::OnTimerWaitOnTargetCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerWaitOnTarget);

	if (bIsSwitchedOn)
	{
		// Move to the other direction
		if (StartTargetLocation == StartPoint)
		{
			TargetLocation = StartPoint;
			StartTargetLocation = EndPoint;

		}
		else
		{
			TargetLocation = EndPoint;
			StartTargetLocation = StartPoint;
		}

		Direction = TargetLocation->GetActorLocation() - StartTargetLocation->GetActorLocation();

		TotalDistance = Direction.Size();
		// Get Normal Direction
		Direction = Direction.GetSafeNormal();
		CurrentDistance = 0.0f;

		bIsMoving = true;
	}
}

void AMovablePlatform::SwitchOn()
{
	if (!bIsSwitchedOn)
	{
		bIsSwitchedOn = true;

		if (!bIsWaiting)
		{
			bIsMoving = true;
		}
	}
}

void AMovablePlatform::SwitchOff()
{
	if (bIsSwitchedOn)
	{
		bIsSwitchedOn = false;

		if (!bIsWaiting)
		{
			bIsMoving = false;
		}
	}
	
}

