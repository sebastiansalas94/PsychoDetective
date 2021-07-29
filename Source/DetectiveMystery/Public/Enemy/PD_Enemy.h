// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PD_Character.h"
#include "PD_Enemy.generated.h"

class APD_PathActor;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_Enemy : public APD_Character
{
	GENERATED_BODY()
	
public:
	APD_Enemy();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI | Navigation Path")
	bool bLoopPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI | Navigation Path")
	int DirectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI | Navigation Path")
	float WaitingTimeOnPathPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Navigation Path")
	APD_PathActor* MyPath;
};
