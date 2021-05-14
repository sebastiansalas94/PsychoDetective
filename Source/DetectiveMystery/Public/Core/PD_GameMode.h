// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PD_GameMode.generated.h"

class APD_Character;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void Victory(APD_Character* Character);

	UFUNCTION()
	void GameOver();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(APD_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver();

};
