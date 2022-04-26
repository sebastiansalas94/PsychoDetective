// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PD_BotSpawnerKey.h"
#include "Enemy/PD_BotSpawner.h"

APD_BotSpawnerKey::APD_BotSpawnerKey()
{
	BotSpawnerKeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBoxMeshComponent"));
	BotSpawnerKeyMeshComponent->SetupAttachment(RootComponent);
	BotSpawnerKeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APD_BotSpawnerKey::PickUp(APD_Character* PickUpCharacter)
{
	Super::PickUp(PickUpCharacter);

	if (IsValid(MyBotSpawner))
	{
		MyBotSpawner->SetIsActive(false);
	}

	Destroy();

}