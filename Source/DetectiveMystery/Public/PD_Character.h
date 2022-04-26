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
class UPD_HealthComponent;
class APD_GameMode;
class UPD_GameInstance;
class UPD_PauseMenuWidget;

UENUM()
enum class EPD_CharacterType : uint8
{
	CharacterType_Player	UMETA(DisplayName = "Player"),
	CharacterType_Enemy	UMETA(DisplayName = "Enemy")
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPD_HealthComponent* HealthComponent;
    
protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
    bool bIsLookInversion;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
    bool bIsFirstPersonView;
    
	UPROPERTY(BlueprintReadOnly, Category = "Melee")
	bool bIsHittingMelee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	bool bCanMakeCombos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	bool bIsComboEnable;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bCanUseWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "StatusAilment")
	bool bIsBurning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Over")
	bool bHasToDestroy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
	bool bUltimateWithTick;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bCanUseUltimate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bIsUsingUltimate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
	float MaxComboMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
	float CurrentComboMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
    float MaxSpeedWalk;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
    float MaxSpeedSprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatusAilment")
	float BurnDamage;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxUltimateXP;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	float CurrentUltimateXP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxUltimateDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Time")
	float CurrentUltimateDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time")
	float UltimateFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time")
	float SlowTimeUltimateFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time")
	float SlowTimeUltimateMinusValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimateWalkSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities")
	float NormalWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimatePlayRate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities")
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimateShotFrequency;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName MeleeSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName BurnStatusSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels")
	FName MainMenuMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	TArray<FName> DoorKeys;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPD_CharacterType CharacterType;

	//TSubclass of es la referencia de la clase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<APD_Weapon> InitialWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ExplosiveBarrel")
	TSubclassOf<UDamageType> DamageTypeSubClass;

	//Esta es la referencia del objeto en escena
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	APD_Weapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* UltimateMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* UltimateSlowTimeMontage;

	UAnimInstance* MyAnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* BurnEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystemComponent* BurnEffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	USoundBase* BurnSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UAudioComponent* BurnSoundComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pause Menu")
	TSubclassOf<UPD_PauseMenuWidget> WidgetPauseMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pause Menu")
	UPD_PauseMenuWidget* WidgetPauseMenuInstance;

	APD_GameMode* GameModeReference;

	UPD_GameInstance* GameInstanceReference;

	//Handle para el timer
	FTimerHandle UnusedHandle;

	FTimerHandle TimerHandleUltimate;

	FTimerHandle TimerHandleAutomaticShoot;

	FTimerHandle TimerHandleBeginUltimateBehavior;

	FTimerHandle TimerHandleBeginSlowTimeUltimateBehavior;

	FTimerHandle TimerHandleGainXPUltimateOverTimeBehavior;

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

	UFUNCTION(BlueprintCallable)
	void StartWeaponAction();
	void StopWeaponAction();

	void StartWeaponSecondaryAction();
	void StopWeaponSecondaryAction();

	UFUNCTION(BlueprintCallable)
	void StartMeleeAction();
	
	void StopMeleeAction();

	void StartUltimate();
	void StopUltimate();

	void StartSlowTimeUltimate();
	void StopSlowTimeUltimate();

	void GoToMainMenu();
	void GoToPauseMenu();

	UFUNCTION()
	void MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    
	UFUNCTION()
	void OnHealthChange(UPD_HealthComponent* CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void AddControllerPitchInput(float value) override;
    
	//TODO - Metodo para interactuar con objetos
	void Interact();

    void AddKey(FName newKey);

	bool TryAddHealth(float healthValue);
    
    bool HasKey(FName keyTag);

	void SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetMeleeState(bool NewState);

	UFUNCTION(BlueprintCallable)
	void SetComboEnable(bool NewState);

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	bool HasToDestroy() { return bHasToDestroy; };

	void BeginBurnState(float NewBurnDamage);

	void EndBurnState();

	UFUNCTION(BlueprintCallable)
	void GainUltimateXP();

	UFUNCTION(BlueprintCallable)
	void GainUltimateXPValue(float XPValue);

	void UpdateUltimateDuration(float Value);

	void UpdateSlowTimeUltimateDuration(float Value);

	void UpdateUltimateDurationWithTimer();

	void UpdateSlowTimeUltimateDurationWithTimer();

	void BeginUltimateBehavior();

	void SlowTime();

	void BeginSlowTimeUltimateBehavior();

	void AddXPUltimateOverTime();

	UFUNCTION(BlueprintCallable)
	EPD_CharacterType GetCharacterType() { return CharacterType; };

	UPD_HealthComponent* GetHealthComponent() { return HealthComponent; }

protected: 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GainUltimateXP(float XPGained);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StopUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_UpdateUltimateDuration(float Value);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimateSoundEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimateVisualEffect();

};
