// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PD_HUDUltimate.h"
#include "Kismet/GameplayStatics.h"
#include "PD_Character.h"

void UPD_HUDUltimate::InitializeWidget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		APD_Character* PlayerCharacter = Cast<APD_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->OnUltimateUpdateDelegate.AddDynamic(this, &UPD_HUDUltimate::UpdateUltimateValue);
			PlayerCharacter->OnUltimateStatusDelegate.AddDynamic(this, &UPD_HUDUltimate::UpdateUltimateStatus);
		}
	}
	UpdateUltimateStatus(false);
}


void UPD_HUDUltimate::UpdateUltimateValue(float CurrentUltimateXP, float MaxUltimateXP)
{
	UltimatePercent = CurrentUltimateXP / MaxUltimateXP;
}

void UPD_HUDUltimate::UpdateUltimateStatus(bool bIsAvailable)
{
	UltimateColor = bIsAvailable ? UltimateEnabledColor : UltimateDisabledColor;
}