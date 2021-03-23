// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_Door.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class DETECTIVEMYSTERY_API APD_Door : public AActor
{
	GENERATED_BODY()
	
    
protected:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    USceneComponent* CustomRootComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    UStaticMeshComponent* DoorFrameComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    UStaticMeshComponent* DoorComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    UBoxComponent* KeyZoneColliderComponent;
    
protected:
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Door")
    float OpenAngle;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Door")
    bool bIsOpen;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Door")
    FName DoorTag;
    
public:	
	// Sets default values for this actor's properties
	APD_Door();
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Door")
    void BP_OpenDoor();
    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void OpenDoor();
    
};
