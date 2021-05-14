// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PD_VictoryZone.h"
#include "Components/BoxComponent.h"
#include "PD_Character.h"
#include "Core/PD_GameMode.h"


// Sets default values
APD_VictoryZone::APD_VictoryZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VictoryZoneComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("VictoryZoneComponent"));
	RootComponent = VictoryZoneComponent;

	VictoryZoneComponent->SetBoxExtent(FVector(100.0f));
}

// Called when the game starts or when spawned
void APD_VictoryZone::BeginPlay()
{
	Super::BeginPlay();
	
	GameModeReference = Cast<APD_GameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void APD_VictoryZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_VictoryZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (IsValid(OtherActor) && IsValid(GameModeReference)) {
		APD_Character* PDCharacter = Cast<APD_Character>(OtherActor);
		if (IsValid(PDCharacter)) {
			GameModeReference->Victory(PDCharacter);
		}
	}

}


