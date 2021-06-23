// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PD_Character.h"

// Sets default values
APD_Door::APD_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    
    CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
    RootComponent = CustomRootComponent;
    
    DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    DoorFrameComponent->SetupAttachment(CustomRootComponent);
    
    DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    DoorComponent->SetupAttachment(CustomRootComponent);
    
    KeyZoneColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("KeyZoneCollider"));
    KeyZoneColliderComponent->SetupAttachment(CustomRootComponent);
    KeyZoneColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    KeyZoneColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    KeyZoneColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    
    OpenAngle = -90.0f;
    
    DoorTag = "KeyA";
}

// Called when the game starts or when spawned
void APD_Door::BeginPlay()
{
	Super::BeginPlay();
    KeyZoneColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &APD_Door::CheckKeyFromPlayer);
}

// Called every frame
void APD_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_Door::OpenDoor()
{
    //FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
    //DoorComponent->SetRelativeRotation(NewRotation);
    
    bIsOpen = true;
    BP_OpenDoor();
}

void APD_Door::CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult){
    
    if(bIsOpen){
        return;
    }
    
    if(IsValid(OtherActor)){
        APD_Character* OverlappedCharacter = Cast<APD_Character>(OtherActor);
        
        if(IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == EPD_CharacterType::CharacterType_Player){
            if(OverlappedCharacter->HasKey(DoorTag)){
                OpenDoor();
            }
        }
    }
}

