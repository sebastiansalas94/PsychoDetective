// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PD_SaveGame.generated.h"

class APD_ElectricalDoor;

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

	UPROPERTY(VisibleDefaultsOnly, Category = "Data")
	int ReactorDestroyedCounter;

	UPROPERTY(VisibleDefaultsOnly, Category = "Data")
	TArray<int> ReactorsDestroyed;

	UPROPERTY(VisibleDefaultsOnly, Category = "Data")
	APD_ElectricalDoor* ElectricalDoor;


public: 

	UFUNCTION(BlueprintCallable)
	void SetEnemiesDefeatedCounterInfo(int NewValue){ EnemiesDefeatedCounter = NewValue; };

	UFUNCTION(BlueprintCallable)
	int GetEnemiesDefeatedCounterInfo(){ return EnemiesDefeatedCounter; };

	UFUNCTION(BlueprintCallable)
	void SetReactorDestroyedCounterInfo(int NewValue) { ReactorDestroyedCounter = NewValue; };

	UFUNCTION(BlueprintCallable)
	int GetReactorDestroyedCounterInfo() { return ReactorDestroyedCounter; };

	UFUNCTION(BlueprintCallable)
	void SetReactorDestroyedArrayInfo(TArray<int> NewValue) { ReactorsDestroyed = NewValue; };

	UFUNCTION(BlueprintCallable)
	APD_ElectricalDoor* GetElectricalDoorInfo() { return ElectricalDoor; };

	UFUNCTION(BlueprintCallable)
	void SetElectricalDoorInfo(APD_ElectricalDoor* NewValue) { ElectricalDoor = NewValue; };

	UFUNCTION(BlueprintCallable)
	TArray<int> GetReactorDestroyedArrayInfo() { return ReactorsDestroyed; };
};
