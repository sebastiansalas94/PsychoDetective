// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PD_Item.h"
#include "PD_Launchpad.h"
#include "PD_SwitchLaunchpadItem.generated.h"

class Launchpad;
class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_SwitchLaunchpadItem : public APD_Item
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SwitchMeshComponent;
    
    UPROPERTY(EditDefaultsOnly)
    class UMaterialInterface* OnMaterial;

    UPROPERTY(EditDefaultsOnly)
    class UMaterialInterface* OffMaterial;
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Launchpad")
    APD_Launchpad* Launchpad;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Launchpad")
    bool bLaunchpadIsActive;
    
public:
    APD_SwitchLaunchpadItem();
    
protected:
    virtual void PickUp(APD_Character* PickUpCharacter) override;
    
    void ChangeMaterial();
};
