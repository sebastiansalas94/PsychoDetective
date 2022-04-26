// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "PD_MainMenuButton.generated.h"

UENUM()
enum class EPD_ButtonStyleType : uint8
{
	ButtonStyleType_Mouse	UMETA(DisplayName = "Selected by Mouse"),
	ButtonStyleType_KeySelected	UMETA(DisplayName = "Selected by Keyboard"),
	ButtonStyleType_KeyNotSelected	UMETA(DisplayName = "Not Selected by Keyboard")
};

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API UPD_MainMenuButton : public UButton
{
	GENERATED_BODY()

public:

	UPD_MainMenuButton();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main Menu Button")
	float CheckFocusRate;

	FTimerHandle TimerHandle_CheckFocus;

protected:

	UFUNCTION(BlueprintCallable, Category = "Main Menu Buttons")
	void Start();

	void CheckFocus();

	void SetButtonStyle(EPD_ButtonStyleType NewStyleType);

	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu Button")
	void BP_SetButtonStyle(EPD_ButtonStyleType NewStyleType);
	
};
