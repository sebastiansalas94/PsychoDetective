// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_LevelMusic.generated.h"

class UBillboardComponent;
class UAudioComponent;
class APD_GameMode;

UCLASS()
class DETECTIVEMYSTERY_API APD_LevelMusic : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* MusicBillboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* MusicAudioComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	FName AlertParamName;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	APD_GameMode* GameModeReference;

public:	
	// Sets default values for this actor's properties
	APD_LevelMusic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void StopLevelMusic();

	UFUNCTION()
	void ChangeLevelMusic(bool bIsAlert);
};