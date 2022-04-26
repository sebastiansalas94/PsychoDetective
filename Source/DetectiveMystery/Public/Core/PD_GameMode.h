// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PD_GameMode.generated.h"

class APD_Character;
class APD_SpectatingCamera;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyAddedSignature, FName, KeyTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStateChange);

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectating Camera")
	float SpectatingBlendTime;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	APD_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	APD_SpectatingCamera* GameOverCamera;

	FTimerHandle TimerHandle_BackToMainMenu;

public: 

	APD_GameMode();

	UPROPERTY(BlueprintAssignable)
	FOnKeyAddedSignature OnKeyAddedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	FName MainMenuMapName;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChange OnVictoryDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChange OnGameOverDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupSpectatingCameras();

	void MoveCameraToSpectatingPoint(APD_Character* Character, APD_SpectatingCamera* SpectatingCamera);

public:

	UFUNCTION()
	void AddKeyToCharacter(APD_Character* KeyOwner, FName KeyTag);

	UFUNCTION()
	void Victory(APD_Character* Character);

	UFUNCTION()
	void GameOver(APD_Character* Character);
	
	void BackToMainMenu();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(APD_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(APD_Character* Character);

};
