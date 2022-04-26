// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_BotSpawner.generated.h"

class UBillboardComponent;
class APD_Bot;
class UParticleSystem;

UCLASS()
class DETECTIVEMYSTERY_API APD_BotSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APD_BotSpawner();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 1.0, ClampMin = 1.0))
	int MaxBotsCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	int CurrentBotsCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 0.1, ClampMin = 0.1))
	float TimeToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> SpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<APD_Bot> BotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	UParticleSystem* CreateBotEffect;

	FTimerHandle TimerHandle_SpawnBot;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* SpawnerBillboardComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnBot();

	FVector GetSpawnPoint();

public:

	void NotifyBotDead();

	bool GetIsActive() { return bIsActive; };

	void SetIsActive(bool NewState) { bIsActive = NewState; };
};
