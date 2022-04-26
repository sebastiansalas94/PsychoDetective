// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PD_HealthBox.h"
#include "Components/StaticMeshComponent.h"
#include "PD_Character.h"

APD_HealthBox::APD_HealthBox()
{
	HealthBoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBoxMeshComponent"));
	HealthBoxMeshComponent->SetupAttachment(RootComponent);
	HealthBoxMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthValue = 15.0f;
}

void APD_HealthBox::PickUp(APD_Character* PickUpCharacter)
{
	Super::PickUp(PickUpCharacter);

	bool bSuccessfulHeal = PickUpCharacter->TryAddHealth(HealthValue);

	if (bSuccessfulHeal)
	{
		Destroy();
	}

}
