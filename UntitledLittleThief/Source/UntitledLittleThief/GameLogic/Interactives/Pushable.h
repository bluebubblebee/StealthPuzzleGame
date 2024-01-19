// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pushable.generated.h"

UCLASS()
class UNTITLEDLITTLETHIEF_API APushable : public AActor
{
	GENERATED_BODY()

protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pushable", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pushable", meta = (AllowPrivateAccess = "true"))
	class UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pushable", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* XDirectionCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pushable", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* YDirectionCollision;
	
public:	
	// Sets default values for this actor's properties
	APushable();

protected:


	bool bIsEnabled = true;

public:
	virtual void SetEnable(bool Enable);

protected:

	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		virtual void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
