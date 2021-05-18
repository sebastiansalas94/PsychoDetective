// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PD_GameMode.h"
#include "PD_Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PD_SpectatingCamera.h"

void APD_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupSpectatingCameras();

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

void APD_GameMode::Victory(APD_Character* Character) {
	Character->DisableInput(nullptr);

	MoveCameraToSpectatingPoint(Character, VictoryCamera);
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

	BP_GameOver(Character);
}