// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PD_HUDReactorCounter.h"
#include "Core/PD_GameInstance.h"

void UPD_HUDReactorCounter::InitializeWidget()
{
	GameInstanceReference = Cast<UPD_GameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->OnReactorDestroyedDelegate.AddDynamic(this, &UPD_HUDReactorCounter::UpdateCounter);
		UpdateCounter(GameInstanceReference->GetReactorDestroyedCounter());
	}
}

void UPD_HUDReactorCounter::UpdateCounter(int ReactorDestroyedCounter)
{
	ReactorsDestroyed = ReactorDestroyedCounter;
}