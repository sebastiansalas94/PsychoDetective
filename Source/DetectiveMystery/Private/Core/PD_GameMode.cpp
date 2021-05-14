// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PD_GameMode.h"
#include "PD_Character.h"

void APD_GameMode::Victory(APD_Character* Character) {
	Character->DisableInput(nullptr);
	BP_Victory(Character);
}

void APD_GameMode::GameOver() {
	BP_GameOver();
}