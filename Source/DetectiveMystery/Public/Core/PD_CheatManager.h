// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "PD_CheatManager.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API UPD_CheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:

	UFUNCTION(exec)
	void PD_UltimateReady();
};
