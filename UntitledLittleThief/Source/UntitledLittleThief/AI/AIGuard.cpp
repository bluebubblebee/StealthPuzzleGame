// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "UntitledLittleThiefCharacter.h"

// Sets default values
AAIGuard::AAIGuard()
{
	// This is not part of the hierarchy
	// It doesn't have a location,
	//https://docs.unrealengine.com/en-US/API/Runtime/AIModule/Perception/UPawnSensingComponent/index.html
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	// Bind sight event and heard event
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIGuard::OnPawnSeen);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIGuard::OnNoiseHeard); 


	CurrentGuardState = EAIState::VE_IDLE;

	PrimaryActorTick.bCanEverTick = true;

}

void AAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
}



void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	

	if (SeenPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AAIGuard::OnPawnSeen] SeenPawn null"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AAIGuard::OnPawnSeen] Seen someone %s"), *SeenPawn->GetName());

		DetectedPlayer = Cast<AUntitledLittleThiefCharacter>(SeenPawn);

		if (DetectedPlayer != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AAIGuard::OnPawnSeen] IsPlayer %s"), *SeenPawn->GetName());
		}
		else
		{
			return;
		}

		SetGuardState(EAIState::VE_ALERT);
		// On player detected
		OnPlayerSeenDetected(DetectedPlayer);
	}


	
	//UE_LOG(LogTemp, Warning, TEXT("[AAIGuard::OnPawnSeen] %s"), *SeenPawn->GetName());


	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12.0f, FColor::Red, false, 10.0f);

	

	
}


void AAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	//UE_LOG(LogTemp, Warning, TEXT("[AAIGuard::OnNoiseHeard] called "));
	if (NoiseInstigator != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AAIGuard::OnNoiseHeard] %s"), *NoiseInstigator->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AAIGuard::OnNoiseHeard] is Null"));
		return;
	}

	//if (CurrentGuardState == EAIState::VE_ALERT) return;

	DetectedPlayer = Cast<AUntitledLittleThiefCharacter>(NoiseInstigator);

	if (DetectedPlayer != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AAIGuard::OnPawnSeen] NoiseMaker %s"), *DetectedPlayer->GetName());

		
		CurrentPatrolPoint = 0;
		OnPlayerHearDetected(DetectedPlayer);

		SetGuardState(EAIState::VE_SUSPICIOUS);

	}
	else
	{
		return;
	}


	


	//DrawDebugSphere(GetWorld(), Location, 32.0f, 12.0f, FColor::Green, false, 10.0f);

	// Rotate the guard on the direction of the noise
	//FVector Direction = Location - GetActorLocation();

	// Rotator from  x direction
	//FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	//NewLookAt.Pitch = 0.0f;
	//NewLookAt.Roll = 0.0f;

	//SetActorRotation(NewLookAt);	

	// Reset orientation with a timer, clear the timer first
	//GetWorldTimerManager().ClearTimer(ResetOrientationTimerHandle);
	//GetWorldTimerManager().SetTimer(ResetOrientationTimerHandle, this, &AAIGuard::ResetOrientation, 3.0f);  




	
	
	

	// Detect player
	//OnPlayerDetected();

}


void AAIGuard::ResetOrientation()
{
	if (CurrentGuardState == EAIState::VE_ALERT) return;

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::VE_IDLE);
}

void AAIGuard::SetGuardState(EAIState NewState)
{
	if (NewState == CurrentGuardState) return;

	CurrentGuardState = NewState;

	// Blueprint event
	OnStateChanged(NewState);
}



AActor*	AAIGuard::GetNextPatrolPoint()
{
	if (PatrolPoint.Num() == 0)
	{
		return nullptr;
	}

	AActor* NextPatrolpoint = PatrolPoint[CurrentPatrolPoint];

	CurrentPatrolPoint += 1;

	if (CurrentPatrolPoint >= PatrolPoint.Num())
	{
		CurrentPatrolPoint = 0;
	}
	   
	return NextPatrolpoint;
}


