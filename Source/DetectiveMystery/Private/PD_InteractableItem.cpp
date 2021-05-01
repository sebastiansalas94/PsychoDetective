// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_InteractableItem.h"
#include "Components/SphereComponent.h"
#include "PD_Character.h"

// Sets default values
APD_InteractableItem::APD_InteractableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainColliderComponent"));
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = MainColliderComponent;
}

// Called when the game starts or when spawned
void APD_InteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_InteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_InteractableItem::Interact(APD_Character* PickUpCharacter) {
	//BP_Interact(PickUpCharacter);
}

void APD_InteractableItem::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsValid(OtherActor)) {
		APD_Character* OverlappedCharacter = Cast<APD_Character>(OtherActor);
		if (IsValid(OverlappedCharacter)) {
			//OverlappedCharacter->CanInteract();
		}
	}
}

