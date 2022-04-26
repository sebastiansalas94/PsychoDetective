// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PD_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API UPD_SaveGame : public USaveGame
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Data")
	int EnemiesDefeatedCounter;

public: 

	UFUNCTION(BlueprintCallable)
	void SetEnemiesDefeatedCounterInfo(int NewValue){ EnemiesDefeatedCounter = NewValue; };

	UFUNCTION(BlueprintCallable)
	int GetEnemiesDefeatedCounterInfo(){ return EnemiesDefeatedCounter; };
};