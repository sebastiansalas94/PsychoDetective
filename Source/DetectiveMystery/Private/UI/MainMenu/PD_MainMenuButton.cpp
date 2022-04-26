// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/PD_MainMenuButton.h"

UPD_MainMenuButton::UPD_MainMenuButton()
{
	CheckFocusRate = 0.1f;
}

void UPD_MainMenuButton::Start()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckFocus, this, &UPD_MainMenuButton::CheckFocus, CheckFocusRate, true);
}

void UPD_MainMenuButton::CheckFocus()
{
	if (IsHovered())
	{
		//This Button is affected by mouse
		SetKeyboardFocus();
		SetButtonStyle(EPD_ButtonStyleType::ButtonStyleType_Mouse);
	}
	else
	{
		//Check if the keyboard/gamepaed is on me!
		EPD_ButtonStyleType StyleTypeSelected = HasKeyboardFocus() ? EPD_ButtonStyleType::ButtonStyleType_KeySelected : EPD_ButtonStyleType::ButtonStyleType_KeyNotSelected;
		SetButtonStyle(StyleTypeSelected);
	}
}

void UPD_MainMenuButton::SetButtonStyle(EPD_ButtonStyleType NewStyleType)
{
	BP_SetButtonStyle(NewStyleType);
}
