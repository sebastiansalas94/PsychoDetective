// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PD_Item.h"
#include "PD_BotSpawnerKey.generated.h"

class APD_BotSpawner;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_BotSpawnerKey : public APD_Item
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BotSpawnerKeyMeshComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bot Spawner")
	APD_BotSpawner* MyBotSpawner;

public:
	APD_BotSpawnerKey();

protected:
	virtual void PickUp(APD_Character* PickUpCharacter) override;

public:
	void SetSpawner(APD_BotSpawner* BotSpawner) { MyBotSpawner = BotSpawner; };

};
