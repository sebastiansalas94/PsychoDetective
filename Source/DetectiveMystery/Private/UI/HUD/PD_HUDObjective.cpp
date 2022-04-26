// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PD_HUDObjective.h"

void UPD_HUDObjective::ObjectiveCompleted()
{
	bIsCompleted = true;
	CurrentColor = CompletedColor;
}
