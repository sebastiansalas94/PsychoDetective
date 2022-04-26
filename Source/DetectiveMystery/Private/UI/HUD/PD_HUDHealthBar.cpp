// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PD_HUDHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "PD_Character.h"
#include "Components/PD_HealthComponent.h"

void UPD_HUDHealthBar::InitializeWidget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		APD_Character* PlayerCharacter = Cast<APD_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			UPD_HealthComponent* PlayerHealthComponent = PlayerCharacter->GetHealthComponent();

			if (IsValid(PlayerHealthComponent))
			{
				PlayerHealthComponent->OnHealthUpdateDelegate.AddDynamic(this, &UPD_HUDHealthBar::UpdateHealth);
			}
		}
	}
}

void UPD_HUDHealthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthPercent = CurrentHealth / MaxHealth;
	HealthColor = FMath::Lerp(EmptyHealthColor, FullHealthColor, HealthPercent);
}
