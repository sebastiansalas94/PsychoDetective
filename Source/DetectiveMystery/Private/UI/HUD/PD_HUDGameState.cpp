// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PD_HUDGameState.h"
#include "Core/PD_GameMode.h"

void UPD_HUDGameState::InitializeWidget()
{
	GameModeReference = Cast<APD_GameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameModeReference))
	{
		GameModeReference->OnVictoryDelegate.AddDynamic(this, &UPD_HUDGameState::OnVictory);
		GameModeReference->OnGameOverDelegate.AddDynamic(this, &UPD_HUDGameState::OnGameOver);
	}

	SetVisibility(ESlateVisibility::Hidden);
}

void UPD_HUDGameState::OnVictory()
{
	GameStateName = VictoryName;
	SetVisibility(ESlateVisibility::Visible);
}

void UPD_HUDGameState::OnGameOver()
{
	GameStateName = GameOverName;
	SetVisibility(ESlateVisibility::Visible);
}
