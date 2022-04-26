// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/PD_MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Core/PD_GameInstance.h"


void UPD_MainMenuWidget::NewGame() 
{
	BP_NewGame();

	UPD_GameInstance* GameInstanceReference = Cast<UPD_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->ResetData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameplayLevelName);
}

void UPD_MainMenuWidget::Continue()
{
	BP_Continue();
	UGameplayStatics::OpenLevel(GetWorld(), GameplayLevelName);
	
	UPD_GameInstance* GameInstanceReference = Cast<UPD_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->LoadData();
	}

}
void UPD_MainMenuWidget::QuitGame()
{
	BP_QuitGame();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
