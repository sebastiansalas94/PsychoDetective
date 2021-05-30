// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_ExplosiveBarrel.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPD_HealthComponent;

UCLASS()
class DETECTIVEMYSTERY_API APD_ExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ExplosiveBarrelMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	USphereComponent* ExplosionColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPD_HealthComponent* HealthComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ExplosiveBarrel")
	float ExplosiveBarrelDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ExplosiveBarrel")
	TSubclassOf<UDamageType> DamageTypeSubClass;

public:	
	// Sets default values for this actor's properties
	APD_ExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyExplosiveBarrel();

	UFUNCTION()
	void OnHealthChange(UPD_HealthComponent* CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Effects")
	void BP_MakeExplodeEffects();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
