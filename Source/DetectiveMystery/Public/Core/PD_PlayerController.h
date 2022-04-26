// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PD_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void ChangeControllerToUI();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_ChangeControllerToUI();


};
