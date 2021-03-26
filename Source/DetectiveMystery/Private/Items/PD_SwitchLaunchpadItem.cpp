// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PD_SwitchLaunchpadItem.h"
#include "PD_Launchpad.h"

APD_SwitchLaunchpadItem::APD_SwitchLaunchpadItem()
{
    SwitchMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMeshComponent"));
    SwitchMeshComponent->SetupAttachment(RootComponent);
    
    TArray<APD_Launchpad*> Comps;
    
    //GetComponents(Comps);
    //if(Comps.Num() > 0)
    //{
    //    Launchpad = Comps[0];
    //}
}

void APD_SwitchLaunchpadItem::PickUp(APD_Character* PickUpCharacter){
    Super::PickUp(PickUpCharacter);
    
    //Launchpad->TurnOnOff();
}
