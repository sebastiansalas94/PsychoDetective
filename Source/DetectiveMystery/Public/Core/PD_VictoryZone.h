// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_VictoryZone.generated.h"

class UBoxComponent;
class APD_GameMode;

UCLASS()
class DETECTIVEMYSTERY_API APD_VictoryZone : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* VictoryZoneComponent;

public:	
	// Sets default values for this actor's properties
	APD_VictoryZone();

protected:

	APD_GameMode* GameModeReference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
