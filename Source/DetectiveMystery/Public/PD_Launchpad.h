// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_Launchpad.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class APD_Character;

UCLASS()
class DETECTIVEMYSTERY_API APD_Launchpad : public AActor
{
	GENERATED_BODY()

protected:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* CustomRootComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* PlatformComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* ZoneColliderComponent;
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Launchpad")
    bool bLaunchpadIsActive;
    
public:	
	// Sets default values for this actor's properties
	APD_Launchpad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void Launch(APD_Character* Character);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Launch")
    void BP_Launch(APD_Character* Character);
    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void TurnOnOff();
    
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    
};
