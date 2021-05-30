// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_LandMine.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPD_HealthComponent;

UCLASS()
class DETECTIVEMYSTERY_API APD_LandMine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APD_LandMine();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	USphereComponent* MainColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LandMineMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPD_HealthComponent* HealthComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LandMine")
	float LandMineDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LandMine")
	float MaxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyLandMine();

	UFUNCTION()
	void BeginAlert(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void StopAlert(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ExplodeLandMine(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnHealthChange(UPD_HealthComponent* CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Effects")
	void BP_MakeAlertSound();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Effects")
	void BP_StopAlertSound();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Effects")
	void BP_MakeExplodeEffects();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
