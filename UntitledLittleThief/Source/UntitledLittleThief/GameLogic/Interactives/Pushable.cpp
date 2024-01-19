// Fill out your copyright notice in the Description page of Project Settings.


#include "Pushable.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/BoxComponent.h"
#include "UntitledLittleThiefCharacter.h"

// Sets default values
APushable::APushable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(false);
	

	// Mesh
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(RootComponent);
	PhysicsConstraint->ComponentName1.ComponentName = "Mesh";


	XDirectionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("XDirectionCollision"));
	XDirectionCollision->SetupAttachment(RootComponent);

	YDirectionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("YDirectionCollision"));
	YDirectionCollision->SetupAttachment(RootComponent);

	XDirectionCollision->OnComponentBeginOverlap.AddDynamic(this, &APushable::BeginOverlap);
	XDirectionCollision->OnComponentEndOverlap.AddDynamic(this, &APushable::EndOverlap);


	YDirectionCollision->OnComponentBeginOverlap.AddDynamic(this, &APushable::BeginOverlap);
	YDirectionCollision->OnComponentEndOverlap.AddDynamic(this, &APushable::EndOverlap);

	bIsEnabled = true;
}

// Called when the game starts or when spawned
void APushable::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void APushable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	

}

void APushable::SetEnable(bool Enable)
{
	bIsEnabled = Enable;

	if (bIsEnabled)
	{
		//YDirectionCollision->SetSimulatePhysics(true);
		//XDirectionCollision->SetSimulatePhysics(true);
		// Only used for spatial queries (raycasts, sweeps, and overlaps).
		YDirectionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		XDirectionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		//YDirectionCollision->SetSimulatePhysics(false);
		//XDirectionCollision->SetSimulatePhysics(false);
		//Remove collision completely
		YDirectionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		XDirectionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void APushable::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap"));


	//if (!bIsEnabled) return;

	AUntitledLittleThiefCharacter* Character = Cast<AUntitledLittleThiefCharacter>(OtherActor);

	if (Character)
	{
		
		Character->SetPushMode(true);

		Mesh->SetSimulatePhysics(true);
		Mesh->SetSimulatePhysics(true);

		//CurrentCharacter->SetInteractiveInRange(this);
	}
}

void APushable::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));

	//if (!bIsEnabled) return;

	AUntitledLittleThiefCharacter* Character = Cast<AUntitledLittleThiefCharacter>(OtherActor);

	if (Character)
	{
		

		Character->SetPushMode(false);

		Mesh->SetSimulatePhysics(false);
		Mesh->SetSimulatePhysics(false);

		//CurrentCharacter->ClearInteractiveInRange(this);
	}
}

