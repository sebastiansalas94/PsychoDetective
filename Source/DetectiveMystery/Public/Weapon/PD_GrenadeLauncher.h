// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PD_Weapon.h"
#include "PD_GrenadeLauncher.generated.h"

class APD_Projectile;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_GrenadeLauncher : public APD_Weapon
{
	GENERATED_BODY()

public:
	APD_GrenadeLauncher();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade Launcher")
	TSubclassOf<APD_Projectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName MuzzleSocketName;

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;
};
