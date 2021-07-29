// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PD_Enemy.h"

APD_Enemy::APD_Enemy()
{
	bLoopPath = false;
	DirectionIndex = 1;
	WaitingTimeOnPathPoint = 1.0f;
}

