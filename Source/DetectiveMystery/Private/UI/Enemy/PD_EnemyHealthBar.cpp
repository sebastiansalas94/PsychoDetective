// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/PD_EnemyHealthBar.h"

void UPD_EnemyHealthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthPercent = CurrentHealth / MaxHealth;
}