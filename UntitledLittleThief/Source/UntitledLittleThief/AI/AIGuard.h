// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGuard.generated.h"


UENUM(BlueprintType) // Guard State
enum class EAIState : uint8
{
	VE_IDLE 		    UMETA(DisplayName = "Idle"),
	VE_SUSPICIOUS		UMETA(DisplayName = "Suspicious"),
	VE_ALERT		    UMETA(DisplayName = "Alert")
};

UCLASS()
class UNTITLEDLITTLETHIEF_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:

	AAIGuard();

protected:

	virtual void BeginPlay() override;

protected:

	// Component for senses 
	UPROPERTY(VisibleAnywhere, Category="Components")
	class UPawnSensingComponent* PawnSensingComponent;

protected:

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume );


	FRotator OriginalRotation;

	FTimerHandle ResetOrientationTimerHandle;
	
	UFUNCTION()
	void ResetOrientation();


protected:
	//UPROPERTY()
	EAIState CurrentGuardState;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);


	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnPlayerSeenDetected(AUntitledLittleThiefCharacter* Pawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnPlayerHearDetected(AUntitledLittleThiefCharacter* Pawn);



// Waipoints
protected:

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bPatrol;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "bPatrol"))
	TArray<AActor*> PatrolPoint;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	int32 CurrentPatrolPoint;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	class AUntitledLittleThiefCharacter* DetectedPlayer;


public: 
	UFUNCTION(BlueprintCallable, Category = "AI") 
	AActor*	GetNextPatrolPoint();


public:	

	virtual void Tick(float DeltaTime) override;


};
