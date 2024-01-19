// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.h"
#include "InteractiveBase.generated.h"



UCLASS()
class UNTITLEDLITTLETHIEF_API AInteractiveBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractiveBase();

protected:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* InteractiveMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* LookAtPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* InteractiveCollision;


public:

	FORCEINLINE USceneComponent* GetLookAtPoint(){ return LookAtPoint; }

	FORCEINLINE bool CanLookAt() { return bCanLookAt; }

	FORCEINLINE UStaticMeshComponent* GetInteractiveMesh() { return InteractiveMesh; }

	FORCEINLINE bool ZoomWhenInRange() { return bZoomWhenInRange; }

	FORCEINLINE EPickupType GetSpectedItem() { return SpectedItem; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Interactive")
	bool bCanLookAt = true;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive")
	bool bZoomWhenInRange = true;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive")
	EPickupType SpectedItem = EPickupType::VE_NONE;

	bool bIsEnabled;

public:
	virtual void SetEnable(bool Enable);

	////////////////// Pickups //////////////////////////////
public:

	UPROPERTY(EditDefaultsOnly, Category = "Interactive Pickup")
	TSubclassOf<class APickupBase> PickupClass;	

	////////////////// Pickups //////////////////////////////

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


protected:

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
public:
	/////// Interactions /////////

	virtual void StartInteract(APawn* PawnInstigator);

	virtual void EndInteract(APawn* PawnInstigator);

	/////// Interactions /////////

protected:
	class AUntitledLittleThiefCharacter* CurrentCharacter;

};
