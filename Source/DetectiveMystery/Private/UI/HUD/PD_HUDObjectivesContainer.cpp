// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PD_HUDObjectivesContainer.h"
#include "Core/PD_GameMode.h"
#include "UI/HUD/PD_HUDObjective.h"
void UPD_HUDObjectivesContainer::InitializeWidget()
{
	GameModeReference = Cast<APD_GameMode>(GetWorld()->GetAuthGameMode());

	if(IsValid(GameModeReference))
	{
		GameModeReference->OnKeyAddedDelegate.AddDynamic(this, &UPD_HUDObjectivesContainer::UpdateObjectives);
	}
}

void UPD_HUDObjectivesContainer::InitializeObjectives(TArray<UPD_HUDObjective*> NewObjectives)
{
	for (UPD_HUDObjective* NewObjective : NewObjectives)
	{
		Objectives.AddUnique(NewObjective);
	}
}

void UPD_HUDObjectivesContainer::UpdateObjectives(FName KeyTag)
{
	int ObjectiveIndex = -1;

	if (KeyTag == "KeyA")
	{
		ObjectiveIndex = 0;
	}

	if (KeyTag == "KeyB")
	{
		ObjectiveIndex = 1;
	}

	if (Objectives.IsValidIndex(ObjectiveIndex))
	{
		UPD_HUDObjective* CurentObjective = Objectives[ObjectiveIndex];
		if (IsValid(CurentObjective))
		{
			CurentObjective->ObjectiveCompleted();
		}
		
	}
}