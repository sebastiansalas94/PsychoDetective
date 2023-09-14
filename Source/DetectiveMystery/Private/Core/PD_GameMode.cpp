// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PD_GameMode.h"
#include "PD_Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PD_SpectatingCamera.h"
#include "Sound/SoundCue.h"
#include "Enemy/PD_Enemy.h"

APD_GameMode::APD_GameMode()
{
	MainMenuMapName = "MainMenuMap";
}

void APD_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupSpectatingCameras();

	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APD_Enemy::StaticClass(), EnemyActors);
	for (AActor* EnemyActor : EnemyActors)
	{
		if (!IsValid(EnemyActor))
		{
			continue;
		}

		APD_Enemy* NewEnemy = Cast<APD_Enemy>(EnemyActor);
		if(IsValid(NewEnemy))
		{
			LevelEnemies.AddUnique(NewEnemy);
		}
	}
}

void APD_GameMode::SetupSpectatingCameras()
{
	TArray<AActor*> SpectatingCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APD_SpectatingCamera::StaticClass(), SpectatingCameraActors);

	if (SpectatingCameraActors.Num() > 0)
	{
		for (AActor* SpectatingActor : SpectatingCameraActors)
		{
			APD_SpectatingCamera* SpectatingCamera = Cast<APD_SpectatingCamera>(SpectatingActor);
			if (IsValid(SpectatingCamera))
			{
				switch (SpectatingCamera->GetCameraType())
				{
				case EPD_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;
				case EPD_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;
				default:
					break;
				}
			}
		}

	}
}

void APD_GameMode::MoveCameraToSpectatingPoint(APD_Character* Character, APD_SpectatingCamera * SpectatingCamera)
{
	if (!IsValid(Character) || !IsValid(SpectatingCamera))
	{
		return;
	}

	AController* CharacterController = Character->GetController();
	if (IsValid(CharacterController))
	{
		APlayerController* PlayerController = Cast<APlayerController>(CharacterController);
		if (IsValid(PlayerController))
		{
			PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
		}
	}

}

void APD_GameMode::PlayMusic(USoundCue* MusicCue)
{
	if (!IsValid(MusicCue))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), MusicCue);

}

void APD_GameMode::AddKeyToCharacter(APD_Character * KeyOwner, FName KeyTag)
{
	if (IsValid(KeyOwner))
	{
		OnKeyAddedDelegate.Broadcast(KeyTag);
		KeyOwner->AddKey(KeyTag);
	}
}

void APD_GameMode::Victory(APD_Character* Character) {
	Character->DisableInput(nullptr);

	MoveCameraToSpectatingPoint(Character, VictoryCamera);
	OnVictoryDelegate.Broadcast();

	PlayMusic(VictoryMusic);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &APD_GameMode::BackToMainMenu, 3.0f, false);

	BP_Victory(Character);
}

void APD_GameMode::GameOver(APD_Character* Character) {
	Character->GetMovementComponent()->StopMovementImmediately();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Character->HasToDestroy())
	{
		Character->DetachFromControllerPendingDestroy();
		Character->SetLifeSpan(5.0f);
	}
	else
	{
		MoveCameraToSpectatingPoint(Character, VictoryCamera);
		MoveCameraToSpectatingPoint(Character, GameOverCamera);
	}

	OnGameOverDelegate.Broadcast();

	PlayMusic(GameOverMusic);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &APD_GameMode::BackToMainMenu, 8.0f, false);
	BP_GameOver(Character);
}

void APD_GameMode::BackToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}

void APD_GameMode::CheckAlertMode()
{
	bool bEnemyInAlertMode = false;

	for (APD_Enemy* EnemyOnLevel : LevelEnemies)
	{
		if (!IsValid(EnemyOnLevel))
		{
			continue;
		}

		if (EnemyOnLevel->IsAlert())
		{
			bEnemyInAlertMode = true;
			break;
		}
	}

	if (bIsAlertMode != bEnemyInAlertMode) 
	{
		bIsAlertMode = bEnemyInAlertMode;
		OnAlertModeChangeDelegate.Broadcast(bIsAlertMode);
	}
}
