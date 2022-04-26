// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PD_Character.h"
#include "PD_Enemy.generated.h"

class APD_PathActor;
class APD_Item;
class APD_AIController;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI| Ultimate Exp")
	float XPValue;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	float LootProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Navigation Path")
	APD_PathActor* MyPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<APD_Item> LootItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Controller")
	APD_AIController* MyAIController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveXP(AActor* DamageCauser);

	bool TrySpawnLoot();

	UFUNCTION()
	void HealthChanged(UPD_HealthComponent* CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser);

public:
	bool GetLoopPath() { return bLoopPath; };
	int GetDirectionIndex() { return DirectionIndex; };
	float GetWaitingTime() { return WaitingTimeOnPathPoint; };
};
