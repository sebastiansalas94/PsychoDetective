// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PD_Item.h"
#include "PD_HealthBox.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_HealthBox : public APD_Item
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HealthBoxMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthValue;

public:
	APD_HealthBox();

public:

	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetHealthValue() const { return HealthValue; };

protected:
	virtual void PickUp(APD_Character* PickUpCharacter) override;
};

