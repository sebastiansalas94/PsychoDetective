// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_Launchpad.h"
#include "Components/BoxComponent.h"
#include "PD_Character.h"

// Sets default values
APD_Launchpad::APD_Launchpad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bLaunchpadIsActive = false;
    
    CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
    RootComponent = CustomRootComponent;
    
    PlatformComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformComponent"));
    PlatformComponent->SetupAttachment(RootComponent);
    
    ZoneColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneCollider"));
    ZoneColliderComponent->SetupAttachment(CustomRootComponent);
    ZoneColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ZoneColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    ZoneColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void APD_Launchpad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_Launchpad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_Launchpad::TurnOnOff(){
    
    if(bLaunchpadIsActive)
        bLaunchpadIsActive = false;
    else if(!bLaunchpadIsActive)
        bLaunchpadIsActive = true;
}

void APD_Launchpad::Launch(APD_Character* Character){
    BP_Launch(Character);
}

void APD_Launchpad::NotifyActorBeginOverlap(AActor* OtherActor){
    Super::NotifyActorBeginOverlap(OtherActor);
    
    if(IsValid(OtherActor)){
        APD_Character* OverlappedCharacter = Cast<APD_Character>(OtherActor);
        if(IsValid(OverlappedCharacter)){
            Launch(OverlappedCharacter);
        }
    }
}

