// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PD_HUDEnemyCounter.h"
#include "Core/PD_GameInstance.h"

void UPD_HUDEnemyCounter::InitializeWidget()
{
	GameInstanceReference = Cast<UPD_GameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->OnEnemyKilledDelegate.AddDynamic(this, &UPD_HUDEnemyCounter::UpdateCounter);
		UpdateCounter(GameInstanceReference->GetEnemiesDefeatedCounter());
	}
}

void UPD_HUDEnemyCounter::UpdateCounter(int EnemyDefeatedCounter)
{
	EnemiesDefeated = EnemyDefeatedCounter;
}
