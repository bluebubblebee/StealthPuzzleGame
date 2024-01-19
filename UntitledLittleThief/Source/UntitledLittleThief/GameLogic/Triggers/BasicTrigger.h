// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicTrigger.generated.h"

UCLASS()
class UNTITLEDLITTLETHIEF_API ABasicTrigger : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	class UBoxComponent* Trigger;
	
public:	

	ABasicTrigger();

protected:

	virtual void BeginPlay() override;


protected:
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void DisableTrigger();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Settings")
	bool bEnableTrigger = true;

	class AUntitledLittleThiefPlayerController* CurrentPlayerController;
};
