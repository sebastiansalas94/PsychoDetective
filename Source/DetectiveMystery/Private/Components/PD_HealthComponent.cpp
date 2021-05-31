// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PD_HealthComponent.h"

// Sets default values for this component's properties
UPD_HealthComponent::UPD_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
}


// Called when the game starts
void UPD_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	MyOwner = GetOwner();
	if (IsValid(MyOwner)) {
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UPD_HealthComponent::TakingDamage);
	}
	
}

void UPD_HealthComponent::TakingDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f || bIsDead) {
		return;
	}
	
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	if (Health == 0.0f) {
		bIsDead = true;
	}

	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bDebug)
	{
		UE_LOG(LogTemp, Log, TEXT("My Health is: %s"), *FString::SanitizeFloat(Health));
	}
}
