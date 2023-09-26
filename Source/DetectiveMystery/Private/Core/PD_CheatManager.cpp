// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PD_CheatManager.h"
#include "Kismet/GameplayStatics.h"
#include "PD_Character.h"

void UPD_CheatManager::PD_UltimateReady()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(IsValid(PlayerPawn))
	{
		APD_Character* PlayerCharacter = Cast<APD_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter) && PlayerCharacter->GetCharacterType() == EPD_CharacterType::CharacterType_Player)
		{
			PlayerCharacter->GainUltimateXPValue(10000);
		}
	}
}