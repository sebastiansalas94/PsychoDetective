// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PD_PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API UPD_PauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pause Menu")
	FName GameplayLevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pause Menu")
	FName MainMenuLevelName;

protected:

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void Continue();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void Reset();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void GoToMainMenu();


protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_Continue();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_Reset();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_GoToMainMenu();

};
