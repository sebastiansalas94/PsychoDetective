// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_Item.generated.h"

class USphereComponent;
class APD_Character;
class APD_GameMode;

UCLASS()
class DETECTIVEMYSTERY_API APD_Item : public AActor
{
	GENERATED_BODY()
	
protected:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    USphereComponent* MainColliderComponent;
    
protected:

	UPROPERTY(BlueprintReadOnly)
	APD_GameMode* GameModeReference;

public:	
	// Sets default values for this actor's properties
	APD_Item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void PickUp(APD_Character* PickUpCharacter);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item")
    void BP_PickUp(APD_Character* PickUpCharacter);
    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    
};
