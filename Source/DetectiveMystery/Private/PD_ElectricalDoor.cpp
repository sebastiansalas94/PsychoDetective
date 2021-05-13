// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_ElectricalDoor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APD_ElectricalDoor::APD_ElectricalDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElecReactorCounter = 0;
	ElecReactorNum = 3;
	OpenAngle = -90.0f;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrameComponent->SetupAttachment(CustomRootComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorComponent->SetupAttachment(CustomRootComponent);
}

// Called when the game starts or when spawned
void APD_ElectricalDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_ElectricalDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_ElectricalDoor::OpenDoor()
{
	bIsOpen = true;
	BP_OpenDoor();
}

void APD_ElectricalDoor::ValidateToOpenDoor()
{
	ElecReactorCounter++;
	if (ElecReactorCounter == ElecReactorNum) {
		OpenDoor();
	}
}
