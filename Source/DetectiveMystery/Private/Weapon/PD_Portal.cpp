// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PD_Portal.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PD_Character.h"

// Sets default values
APD_Portal::APD_Portal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanTeleport = true;

	PortalCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PortalCollision"));
	RootComponent = PortalCollision;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(PortalCollision);
}

// Called when the game starts or when spawned
void APD_Portal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APD_Portal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APD_Portal::AssignOtherPortal(APD_Portal* OtherPortal) {
	if (IsValid(OtherPortal)) {
		OtherPortalInLevel = OtherPortal;
		bCanTeleport = true;
	}
}

void APD_Portal::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsValid(OtherActor)) {
		APD_Character* OverlappedCharacter = Cast<APD_Character>(OtherActor);
		if (IsValid(OverlappedCharacter) && IsValid(OtherPortalInLevel) && bCanTeleport) {
			TurnOffPortal();

			OverlappedCharacter->TeleportTo(OtherPortalInLevel->GetActorLocation(), OverlappedCharacter->GetActorRotation());

			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APD_Portal::TurnOnPortal, 0.5, false);
		}
	}
}

void APD_Portal::TurnOnPortal() {
	bCanTeleport = true;
	if (IsValid(OtherPortalInLevel)) {
		OtherPortalInLevel->bCanTeleport = true;
	}
}

void APD_Portal::TurnOffPortal() {
	bCanTeleport = false;
	if (IsValid(OtherPortalInLevel)) {
		OtherPortalInLevel->bCanTeleport = false;
	}
}