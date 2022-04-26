// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_ElectricalDoor.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class APD_ElectricalReactor;

UCLASS()
class DETECTIVEMYSTERY_API APD_ElectricalDoor : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	UStaticMeshComponent* DoorFrameComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	UStaticMeshComponent* DoorComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Electrical Door")
	float OpenAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Electrical Door")
	bool bIsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Electrical Door")
	int ElecReactorCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Electrical Door")
	int ElecReactorNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Electrical Door")
	TArray<APD_ElectricalReactor*> ElecReactorArray;


public:	
	// Sets default values for this actor's properties
	APD_ElectricalDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Electrical Door")
	void BP_OpenDoor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenDoor();

	void ValidateToOpenDoor();

	TArray<APD_ElectricalReactor*> GetElecReactors() { return ElecReactorArray; };
};
