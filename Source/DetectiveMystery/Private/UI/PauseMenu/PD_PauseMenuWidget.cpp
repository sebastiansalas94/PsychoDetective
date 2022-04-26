// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu/PD_PauseMenuWidget.h"
#include "Core/PD_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Core/PD_PlayerController.h"

void UPD_PauseMenuWidget::Continue()
{
	BP_Continue();

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();

	APD_PlayerController* MyPlayerController = Cast<APD_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(MyPlayerController))
	{
		MyPlayerController->SetInputMode(FInputModeGameOnly());
		MyPlayerController->SetShowMouseCursor(false);
	}
}

void UPD_PauseMenuWidget::Reset()
{
	BP_Reset();

	UPD_GameInstance* GameInstanceReference = Cast<UPD_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->ResetData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameplayLevelName);
}

void UPD_PauseMenuWidget::GoToMainMenu()
{
	BP_GoToMainMenu();

	UPD_GameInstance* GameInstanceReference = Cast<UPD_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->SaveData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
}