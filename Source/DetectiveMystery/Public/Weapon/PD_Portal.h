// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_Portal.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class DETECTIVEMYSTERY_API APD_Portal : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PortalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* PortalCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	APD_Portal* OtherPortalInLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	bool bCanTeleport;

	//Handle para el timer
	FTimerHandle UnusedHandle;

public:	
	// Sets default values for this actor's properties
	APD_Portal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AssignOtherPortal(APD_Portal* OtherPortal);

	void TurnOnPortal();

	void TurnOffPortal();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};