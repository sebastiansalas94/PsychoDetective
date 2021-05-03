// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PD_Portal.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APD_Portal::APD_Portal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


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

