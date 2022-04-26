
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PD_GameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyKilledSignature, int, EnemiesDefeated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReactorDestroyedSignature, int, ReactorDestroyed);

class APD_ElectricalReactor;
class APD_ElectricalDoor;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API UPD_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPD_GameInstance();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save System")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Data")
	int EnemiesDefeatedCounter;

	UPROPERTY(VisibleAnywhere, Category = "Reactor Data")
	int ReactorsDestroyedCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Reactor Data")
	TArray<int> ReactorsDestroyed;

	UPROPERTY(BlueprintReadOnly, Category = "Reactor Data")
	APD_ElectricalDoor* ElectricalDoorRef;

public:

	UPROPERTY(BlueprintAssignable)
	FOnEnemyKilledSignature OnEnemyKilledDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnReactorDestroyedSignature OnReactorDestroyedDelegate;
	
public:

	UFUNCTION(BlueprintCallable)
	int GetEnemiesDefeatedCounter() { return  EnemiesDefeatedCounter; };
	
	UFUNCTION(BlueprintCallable)
	void SetEnemiesDefeatedCounter(int NewValue) { EnemiesDefeatedCounter = NewValue; };

	UFUNCTION(BlueprintCallable)
	void AddEnemyDefeatedToCounter();

	UFUNCTION(BlueprintCallable)
	int GetReactorDestroyedCounter() { return  ReactorsDestroyedCounter; };

	UFUNCTION(BlueprintCallable)
	void SetReactorDestroyedCounter(int NewValue) { ReactorsDestroyedCounter = NewValue; };

	UFUNCTION(BlueprintCallable)
	void AddReactorDestroyedToCounter(int index);

	UFUNCTION(BlueprintCallable)
	TArray<int> GetReactorDestroyedArray() { return  ReactorsDestroyed; };

	UFUNCTION(BlueprintCallable)
	void SaveData();

	UFUNCTION(BlueprintCallable)
	void LoadData();

	UFUNCTION(BlueprintCallable)
	void ResetData();

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void BP_AddEnemyDefeatedToCounter();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_AddReactorDestroyedToCounter();

	UFUNCTION(BlueprintImplementableEvent)
	AActor* BP_GetElectricalDoor();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_SaveData();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_LoadData();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ResetData();

};
