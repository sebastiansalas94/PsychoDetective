// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PD_Item.h"
#include "PD_DoorKey.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_DoorKey : public APD_Item
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* KeyMeshComponent;
    
protected:
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
    FName KeyTag;
    
public:
    APD_DoorKey();
    
public:
    
    UFUNCTION(BlueprintCallable, Category = "Getter")
    FName GetKeyTag() const { return KeyTag; };

protected:
    virtual void PickUp(APD_Character* PickUpCharacter) override;
};
