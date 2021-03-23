// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PD_DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "PD_Character.h"

APD_DoorKey::APD_DoorKey()
{
    KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
    KeyMeshComponent->SetupAttachment(RootComponent);
    //KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    KeyTag = "KeyA";
}

void APD_DoorKey::PickUp(APD_Character* PickUpCharacter){
    Super::PickUp(PickUpCharacter);
    
    PickUpCharacter->AddKey(KeyTag);
    Destroy();
}
