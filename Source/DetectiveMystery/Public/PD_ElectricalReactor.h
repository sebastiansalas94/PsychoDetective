// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_ElectricalReactor.generated.h"

class APD_ElectricalDoor;
class USceneComponent;
class UStaticMeshComponent;
class UPD_GameInstance;

UCLASS()
class DETECTIVEMYSTERY_API APD_ElectricalReactor : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	UStaticMeshComponent* ElecReactorComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Electrical Reactor")
	int ReactorIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Electrical Reactor")
	bool bIsWorking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Electrical Reactor")
	APD_ElectricalDoor* ElectricalDoor;

	UPD_GameInstance* GameInstanceReference;

public:
	// Sets default values for this actor's properties
	APD_ElectricalReactor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeReferences();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Electrical Reactor")
	void BP_SpawnEffectExplosion();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Electrical Reactor")
	void BP_ChangeMaterialExploded();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	int GetReactorIndex() { return ReactorIndex; };

	void SetIsWorking(bool NewState) { bIsWorking = NewState; };

	void ExplodeReactor();
};
