// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PD_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/PD_SaveGame.h"
#include "PD_ElectricalReactor.h"
#include "PD_ElectricalDoor.h"
#include "Containers/Array.h"


UPD_GameInstance::UPD_GameInstance()
{
	SaveSlotName = "PD_SaveData";
}

void UPD_GameInstance::AddEnemyDefeatedToCounter()
{
	EnemiesDefeatedCounter++;
	OnEnemyKilledDelegate.Broadcast(EnemiesDefeatedCounter);
	BP_AddEnemyDefeatedToCounter();
}

void UPD_GameInstance::AddReactorDestroyedToCounter(int reactorIndex)
{
	ReactorsDestroyedCounter++;
	OnReactorDestroyedDelegate.Broadcast(ReactorsDestroyedCounter);
	ReactorsDestroyed.Add(reactorIndex);
	BP_AddReactorDestroyedToCounter();
}

void UPD_GameInstance::SaveData()
{
	USaveGame* SaveGameObject = nullptr;
	bool bExistData = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);
	if (bExistData)
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(UPD_SaveGame::StaticClass());
	}

	if (IsValid(SaveGameObject))
	{
		UPD_SaveGame* SaveFile = Cast<UPD_SaveGame>(SaveGameObject);
		if (IsValid(SaveFile))
		{
			SaveFile->SetEnemiesDefeatedCounterInfo(EnemiesDefeatedCounter);
			SaveFile->SetReactorDestroyedCounterInfo(ReactorsDestroyedCounter);
			SaveFile->SetReactorDestroyedArrayInfo(ReactorsDestroyed);
			
			UGameplayStatics::SaveGameToSlot(SaveFile, SaveSlotName, 0);
		}
	}
}

void UPD_GameInstance::LoadData()
{
	USaveGame* SaveGameObject = nullptr;
	bool bExistData = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);
	if (bExistData)
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	}

	if (IsValid(SaveGameObject))
	{
		UPD_SaveGame* SaveFile = Cast<UPD_SaveGame>(SaveGameObject);
		if (IsValid(SaveFile))
		{
			EnemiesDefeatedCounter = SaveFile->GetEnemiesDefeatedCounterInfo();
		}
	}
}

void UPD_GameInstance::ResetData()
{
	EnemiesDefeatedCounter = 0;
	ReactorsDestroyedCounter = 0;
	ReactorsDestroyed.Empty();
	BP_ResetData();
}