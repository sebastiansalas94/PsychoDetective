// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PD_Bot.generated.h"

class UStaticMeshComponent;
class APD_Character;
class UPD_HealthComponent;
class USphereComponent;
class UParticleSystem;
class APD_Item;
class APD_BotSpawner;
class UPD_GameInstance;

UCLASS()
class DETECTIVEMYSTERY_API APD_Bot : public APawn
{
	GENERATED_BODY()

public: 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BotMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SelfDestructionDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPD_HealthComponent* HealthComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Self Destruction")
	bool bIsExploded;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Self Destruction")
	bool bIsStartingCountdown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movements")
	float MinDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movements")
	float ForceMagnitude;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bot Ultimate Exp")
	float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	float LootProbability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	float BotSpawnerKeyLootProbability;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	APD_BotSpawner* MySpawner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<APD_Item> LootItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<APD_Item> BotSpawnerKeyLootItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "Bot")
	FVector NextPathPoint;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	APD_Character* PlayerCharacter;

	UMaterialInstanceDynamic* BotMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	UParticleSystem* ExplosionEffect;

	UPD_GameInstance* GameInstanceReference;

	FTimerHandle TimerHandle_SelfDamage;

public:

	// Sets default values for this pawn's properties
	APD_Bot();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	FVector GetNextPathPoint();

	UFUNCTION()
	void TakingDamage(UPD_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void SelfDestruction();

	UFUNCTION()
	void StartCountDown(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void SelfDamage();

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveXP(AActor* DamageCauser);

	bool TrySpawnLoot();

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSpawner(APD_BotSpawner* NewSpawner) { MySpawner = NewSpawner; };

};
