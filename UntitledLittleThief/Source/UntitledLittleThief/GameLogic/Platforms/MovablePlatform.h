// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovablePlatform.generated.h"

UCLASS()
class UNTITLEDLITTLETHIEF_API AMovablePlatform : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovablePlatform")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovablePlatform")
	class UStaticMeshComponent* Mesh;

protected:

	UPROPERTY(EditInstanceOnly, Category = "Movable Platform Settings")
	class AActor* StartPoint;

	UPROPERTY(EditInstanceOnly, Category = "Movable Platform Settings")
	class AActor* EndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movable Platform Settings")
	float Speed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movable Platform Settings")
	float WaitTimeOnTarget = 5.0f;
	
public:	

	AMovablePlatform();

protected:

	virtual void BeginPlay() override;


protected:
	bool bIsSwitchedOn = false;

	
	bool bIsMoving = false;

	FVector Direction;
	float TotalDistance;
	float CurrentDistance;

	AActor* StartTargetLocation;
	AActor* TargetLocation;

protected:

	bool bIsWaiting = false;

	FTimerHandle TimerWaitOnTarget;

	UFUNCTION()
	void OnTimerWaitOnTargetCompleted();

public:	

	virtual void Tick(float DeltaTime) override;

public:
	
	void SwitchOn();

	void SwitchOff();

};
