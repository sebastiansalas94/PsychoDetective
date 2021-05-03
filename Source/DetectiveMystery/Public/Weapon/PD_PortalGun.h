// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PD_Weapon.h"
#include "PD_PortalGun.generated.h"

class APD_Portal;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_PortalGun : public APD_Weapon
{
	GENERATED_BODY()
	
public:
	APD_PortalGun();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Portal")
	TSubclassOf<APD_Portal> PortalClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Portal")
	APD_Portal* CurrentPortal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
	float TraceLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* TraceEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName TraceParamName;

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;

	virtual void StartSecondaryAction() override;
};
