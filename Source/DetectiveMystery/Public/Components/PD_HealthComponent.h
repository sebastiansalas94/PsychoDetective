// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PD_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangeSignature, UPD_HealthComponent*, HealthComponent, AActor *, DamagedActor, float, Damage, const UDamageType *, DamageType, AController *, InstigatedBy, AActor *, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadSignature, AActor*, DamageCause);

UCLASS( ClassGroup=(PSYCHO), meta=(BlueprintSpawnableComponent) )
class DETECTIVEMYSTERY_API UPD_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
	bool bIsDefenseUp;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
	bool bIsDead;

	UPROPERTY(BlueprintReadWrite, Category = "Health Component")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component", meta = (ClampMin = 0.0, UIMin = 0.0 ))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
	AActor* MyOwner;

public: 

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeSignature OnHealthChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDeadSignature OnDeadDelegate;

public:	
	// Sets default values for this component's properties
	UPD_HealthComponent();

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bIsDead; };

	void SetMaxHealth(float NewMaxHealth) { MaxHealth = NewMaxHealth; };

	bool HealHealth(float CureValue);

	float GetMaxHealth() { return MaxHealth; };

	float GetCurrentHealth() { return Health; };

	void SetDefenseUpBuff();

	void EndDefenseUpBuff();

	bool GetDefenseUp() { return bIsDefenseUp; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
	UFUNCTION()
	void TakingDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
