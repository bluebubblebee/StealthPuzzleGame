// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserBeamCrossing.generated.h"

UCLASS()
class UNTITLEDLITTLETHIEF_API ALaserBeamCrossing : public AActor
{
	GENERATED_BODY()

protected:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserBeamCrossing")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserBeamCrossing")
	class USceneComponent* BeamSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserBeamCrossing")
	class USceneComponent* TargetSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserBeamCrossing")
	class UBoxComponent* BeamCollision;

protected:


	UPROPERTY(EditDefaultsOnly, Category = "LaserBeamCrossing")
	UParticleSystem* BeamParticleSystem;

	UParticleSystemComponent* BeamParticleSystemComponent; 

protected:

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	//FVector CurrentBeamSourcePoint;
	FVector CurrentTargetPoint;

	FVector LocalTargetLocation;
	FVector LocalBeamSourcePointLocation;
	
public:	

	ALaserBeamCrossing();

protected:

	virtual void BeginPlay() override;



};
