// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_SpectatingCamera.generated.h"

class UStaticMeshComponent;

UENUM()
enum class EPD_SpectatingCameraType : uint8
{
	CameraType_None		UMETA(DisplayName = "None"),
	CameraType_Victory	UMETA(DisplayName = "Victory"),
	CameraType_GameOver	UMETA(DisplayName = "Game Over")
};

UCLASS()
class DETECTIVEMYSTERY_API APD_SpectatingCamera : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SpectatingCameraComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spectating Camera")
	EPD_SpectatingCameraType CameraType;

public:	
	// Sets default values for this actor's properties
	APD_SpectatingCamera();

	EPD_SpectatingCameraType GetCameraType() { return CameraType; };
};
