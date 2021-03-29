// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PD_SwitchLaunchpadItem.h"
#include "PD_Launchpad.h"

APD_SwitchLaunchpadItem::APD_SwitchLaunchpadItem()
{
    SwitchMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMeshComponent"));
    SwitchMeshComponent->SetupAttachment(RootComponent);
    
    OnMaterial = CreateDefaultSubobject<UMaterial>(TEXT("OnMaterial"));
    OffMaterial = CreateDefaultSubobject<UMaterial>(TEXT("OffMaterial"));
    
    SwitchMeshComponent->SetMaterial(0, OffMaterial);
    bLaunchpadIsActive = false;
    ChangeMaterial();
}

void APD_SwitchLaunchpadItem::PickUp(APD_Character* PickUpCharacter){
    Super::PickUp(PickUpCharacter);
    
    APD_Launchpad* LaunchpadReference = Cast<APD_Launchpad>(Launchpad);
    if(IsValid(LaunchpadReference)){
        bLaunchpadIsActive = Launchpad->TurnOnOff();
        ChangeMaterial();
    }
}

void APD_SwitchLaunchpadItem::ChangeMaterial(){
    if(bLaunchpadIsActive)
        SwitchMeshComponent->SetMaterial(0, OnMaterial);
    else
        SwitchMeshComponent->SetMaterial(0, OffMaterial);
}

