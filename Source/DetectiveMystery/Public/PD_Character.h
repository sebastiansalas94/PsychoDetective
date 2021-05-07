// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PD_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class APD_Weapon;
class UAnimMontage;
class UAnimInstance;

UCLASS()
class DETECTIVEMYSTERY_API APD_Character : public ACharacter
{
    GENERATED_BODY()

protected:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* FPSCameraComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* TPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* MeleeDetectorComponent;
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
    bool bIsLookInversion;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
    bool bIsFirstPersonView;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
    FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName MeleeSocketName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
    TArray<FName> DoorKeys;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
    float MaxSpeedWalk;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
    float MaxSpeedSprint;
    
	//TSubclass of es la referencia de la clase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<APD_Weapon> InitialWeaponClass;

	//Esta es la referencia del objeto en escena
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	APD_Weapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MeleeMontage;

	UAnimInstance* MyAnimInstance;

public:
    // Sets default values for this character's properties
    APD_Character();

	virtual FVector GetPawnViewLocation() const override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

	void InitiliazeReferences();

    void MoveForward(float value);
    void MoveRight(float value);
    
    virtual void Jump() override;
    virtual void StopJumping() override;
    
    void StartSprint();
    void StopSprint();

	void CreateInitialWeapon();

	void StartWeaponAction();
	void StopWeaponAction();

	void StartWeaponSecondaryAction();
	void StopWeaponSecondaryAction();

	void StartMeleeAction();
	void StopMeleeAction();
    
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void AddControllerPitchInput(float value) override;
    
	//TODO - Metodo para interactuar con objetos
	void Interact();

    void AddKey(FName newKey);
    
    bool HasKey(FName keyTag);
};
