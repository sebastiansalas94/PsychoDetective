// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PD_HUDGameState.generated.h"

class APD_GameMode;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API UPD_HUDGameState : public UUserWidget
{
	GENERATED_BODY()
	
protected: 

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	FName GameStateName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	FName VictoryName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	FName GameOverName;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	APD_GameMode* GameModeReference;


public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION()
	void OnVictory();

	UFUNCTION()
	void OnGameOver();
};
