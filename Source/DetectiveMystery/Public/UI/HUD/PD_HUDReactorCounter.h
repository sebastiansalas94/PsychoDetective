// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PD_HUDReactorCounter.generated.h"

class UPD_GameInstance;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API UPD_HUDReactorCounter : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	int ReactorsDestroyed;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	UPD_GameInstance* GameInstanceReference;

public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION()
	void UpdateCounter(int ReactorDestroyedCounter);

};
