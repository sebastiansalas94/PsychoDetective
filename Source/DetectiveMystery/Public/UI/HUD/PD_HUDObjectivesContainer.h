// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PD_HUDObjectivesContainer.generated.h"

class APD_GameMode;
class UPD_HUDObjective;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API UPD_HUDObjectivesContainer : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	APD_GameMode* GameModeReference;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	TArray<UPD_HUDObjective*> Objectives;

public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION(BlueprintCallable)
	void InitializeObjectives(TArray<UPD_HUDObjective*> NewObjectives);

	UFUNCTION()
	void UpdateObjectives(FName KeyTag);


};
