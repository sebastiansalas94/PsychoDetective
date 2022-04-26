// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PD_DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "PD_Character.h"
#include "Core/PD_GameMode.h"

APD_DoorKey::APD_DoorKey()
{
    KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
    KeyMeshComponent->SetupAttachment(RootComponent);
    //KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
	XPValue = 40.0f;
    KeyTag = "KeyA";
}

void APD_DoorKey::PickUp(APD_Character* PickUpCharacter){
    Super::PickUp(PickUpCharacter);
    
	if (IsValid(PickUpCharacter) && PickUpCharacter->GetCharacterType() == EPD_CharacterType::CharacterType_Player)
	{
		if (IsValid(GameModeReference))
		{
			GameModeReference->AddKeyToCharacter(PickUpCharacter, KeyTag);
		}
		Destroy();
	}
}
