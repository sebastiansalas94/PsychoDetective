// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_InteractableItem.generated.h"

class USphereComponent;
class APD_Character;

UCLASS()
class DETECTIVEMYSTERY_API APD_InteractableItem : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	USphereComponent* MainColliderComponent;

public:	
	// Sets default values for this actor's properties
	APD_InteractableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact(APD_Character* PickUpCharacter);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
