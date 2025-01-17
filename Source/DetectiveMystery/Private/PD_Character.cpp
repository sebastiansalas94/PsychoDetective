// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_Character.h"
#include "../DetectiveMystery.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PD_Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PD_HealthComponent.h"
#include "Core/PD_GameMode.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Core/PD_GameInstance.h"
#include "UI/PauseMenu/PD_PauseMenuWidget.h"
#include "Core/PD_PlayerController.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
APD_Character::APD_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraSocketName = "SCK_Camera";
	MeleeSocketName = "SCK_Melee";
	BurnStatusSocketName = "pelvis";
	bIsFirstPersonView = true;
	bCanUseWeapon = true;
	MaxSpeedWalk = 600;
	MaxSpeedSprint = 1000;
	MeleeDamage = 10.0f;

	bUltimateWithTick = true;
	MaxUltimateXP = 100.0f;
	MaxUltimateDuration = 10.0f;
	UltimateFrequency = 0.5f;

	MaxComboMultiplier = 4;
	CurrentComboMultiplier = 1;

	MainMenuMapName = "MainMenuMap";

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(SpringArmComponent);

	MeleeDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeDetectorComponent"));
	MeleeDetectorComponent->SetupAttachment(GetMesh(), MeleeSocketName);
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_MELEE, ECR_Overlap);
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<UPD_HealthComponent>(TEXT("HealthComponent"));

	StepSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StepSoundComponent"));
	StepSoundComponent->SetupAttachment(RootComponent);

	VoiceSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceSoundComponent"));
	VoiceSoundComponent->SetupAttachment(RootComponent);

	UltimateWalkSpeed = 1000.0f;
	UltimatePlayRate = 2.0f;
	PlayRate = 1.0f;
	UltimateShotFrequency = 0.25f;

	SlowTimeUltimateFrequency = 0.2f;
	SlowTimeUltimateMinusValue = 1.0f;
}

FVector APD_Character::GetPawnViewLocation() const
{
	if (IsValid(FPSCameraComponent) && bIsFirstPersonView) {
		return FPSCameraComponent->GetComponentLocation();
	}
	else if (IsValid(TPSCameraComponent) && !bIsFirstPersonView) {
		return TPSCameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
	
}

// Called when the game starts or when spawned
void APD_Character::BeginPlay()
{
    Super::BeginPlay();
	InitiliazeReferences();
	CreateInitialWeapon();
	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &APD_Character::MakeMeleeDamage);

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &APD_Character::OnHealthChange);

	NormalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	//AddXPUltimateOverTime();
}

void APD_Character::InitiliazeReferences() {
	if (IsValid(GetMesh())) {
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	GameModeReference = Cast<APD_GameMode>(GetWorld()->GetAuthGameMode());
	GameInstanceReference = Cast<UPD_GameInstance>(GetWorld()->GetGameInstance());
}

void APD_Character::MoveForward(float value){
    AddMovementInput(GetActorForwardVector() * value);
}

void APD_Character::MoveRight(float value){
    AddMovementInput(GetActorRightVector() * value);
}

void APD_Character::Jump(){
    Super::Jump();
}

void APD_Character::StopJumping(){
    Super::StopJumping();
}

void APD_Character::StartSprint(){
    GetCharacterMovement()->MaxWalkSpeed = MaxSpeedSprint;
}

void APD_Character::StopSprint(){
    GetCharacterMovement()->MaxWalkSpeed  = MaxSpeedWalk;
}

void APD_Character::CreateInitialWeapon() {

	if (IsValid(InitialWeaponClass)) {
		CurrentWeapon = GetWorld()->SpawnActor<APD_Weapon>(InitialWeaponClass, GetActorLocation(), GetActorRotation());

		if (IsValid(CurrentWeapon)) {
			CurrentWeapon->SetCharacterOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
	}
}

void APD_Character::StartWeaponAction() {

	if (!bCanUseWeapon) {
		return;
	}
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartAction();

		if (bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandleAutomaticShoot, CurrentWeapon, &APD_Weapon::StartAction, UltimateShotFrequency, true);
		}

	}
}

void APD_Character::StopWeaponAction() {

	if (!bCanUseWeapon) {
		return;
	}
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StopAction();

		if (bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandleAutomaticShoot);
		}
	}
}

void APD_Character::StartWeaponSecondaryAction() {

	if (!bCanUseWeapon) {
		return;
	}
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartSecondaryAction();
	}
}

void APD_Character::StopWeaponSecondaryAction() {

	if (!bCanUseWeapon) {
		return;
	}
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StopSecondaryAction();
	}
}

void APD_Character::StartMeleeAction() {

	if (bIsHittingMelee && !bCanMakeCombos) {
		return;
	}

	if (bCanMakeCombos) {

		if (bIsHittingMelee) {

			if (bIsComboEnable) {
				if (CurrentComboMultiplier < MaxComboMultiplier) {
					CurrentComboMultiplier++;
					SetComboEnable(false);
				}
				else {
					return;
				}
			}
			else {
				return;
			}
		}
	}

	if (IsValid(MyAnimInstance) && IsValid(MeleeMontage)) {
		MyAnimInstance->Montage_Play(MeleeMontage, PlayRate);
	}
	SetMeleeState(true);
}

void APD_Character::StopMeleeAction() {
	UE_LOG(LogTemp, Warning, TEXT("Stop melee action"));
}

void APD_Character::MakeMeleeDamage(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsValid(OtherActor)) {

		if (OtherActor == this)
		{
			return;
		}

		APD_Character* MeleeTarget = Cast<APD_Character>(OtherActor);
		UE_LOG(LogTemp, Verbose, TEXT("MeleeTarget: %s"), MeleeTarget);
		if (IsValid(MeleeTarget))
		{
			bool bPlayerAttackingEnemy = GetCharacterType() == EPD_CharacterType::CharacterType_Player && MeleeTarget->GetCharacterType() == EPD_CharacterType::CharacterType_Enemy;
			bool bEnemyAttackingPlayer = GetCharacterType() == EPD_CharacterType::CharacterType_Enemy && MeleeTarget->GetCharacterType() == EPD_CharacterType::CharacterType_Player;

			UE_LOG(LogTemp, Verbose, TEXT("bPlayerAttackingEnemy: %b"), bPlayerAttackingEnemy);
			UE_LOG(LogTemp, Verbose, TEXT("bEnemyAttackingPlayer: %b"), bEnemyAttackingPlayer);

			if (bPlayerAttackingEnemy || bEnemyAttackingPlayer)
			{
				UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
			}
		}
		else
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
		}


	}
}

void APD_Character::OnHealthChange(UPD_HealthComponent * CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (!HealthComponent->IsDead())
	{
		PlayVoiceSound(HurtSound);
	}

	if (HealthComponent->IsDead())
	{
		PlayVoiceSound(DeadSound);

		if (GetCharacterType() == EPD_CharacterType::CharacterType_Player)
		{
			if (IsValid(GameModeReference))
			{
				GameModeReference->GameOver(this);
			}
		}
	}
}

void APD_Character::AddControllerPitchInput(float value){
    Super::AddControllerPitchInput(bIsLookInversion ? -value : value);
}

// Called every frame
void APD_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	if (bIsBurning)
	{
		UGameplayStatics::ApplyDamage(this, BurnDamage * FApp::GetDeltaTime(), GetInstigatorController(), this, DamageTypeSubClass);
	}

	if (bUltimateWithTick && bIsUsingUltimate) {
		UpdateUltimateDuration(DeltaTime);
	}

}

// Called to bind functionality to input
void APD_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("MoveForward", this, &APD_Character::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APD_Character::MoveRight);
    
    PlayerInputComponent->BindAxis("LookUp", this, &APD_Character::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput);
    
    PlayerInputComponent-> BindAction("Jump", IE_Pressed, this, &APD_Character::Jump);
    PlayerInputComponent-> BindAction("Jump", IE_Released, this, &APD_Character::StopJumping);
    
    PlayerInputComponent-> BindAction("Sprint", IE_Pressed, this, &APD_Character::StartSprint);
    PlayerInputComponent-> BindAction("Sprint", IE_Released, this, &APD_Character::StopSprint);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APD_Character::Interact);

	PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &APD_Character::StartWeaponAction);
	PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &APD_Character::StopWeaponAction);

	PlayerInputComponent->BindAction("WeaponSecondaryAction", IE_Pressed, this, &APD_Character::StartWeaponSecondaryAction);
	PlayerInputComponent->BindAction("WeaponSecondaryAction", IE_Released, this, &APD_Character::StopWeaponSecondaryAction);

	PlayerInputComponent->BindAction("MeleeAction", IE_Pressed, this, &APD_Character::StartMeleeAction);
	PlayerInputComponent->BindAction("MeleeAction", IE_Released, this, &APD_Character::StopMeleeAction);

	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &APD_Character::StartSlowTimeUltimate);
	PlayerInputComponent->BindAction("Ultimate", IE_Released, this, &APD_Character::StopSlowTimeUltimate);

	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &APD_Character::GoToMainMenu);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APD_Character::GoToPauseMenu);
}

void APD_Character::AddKey(FName newKey){
    DoorKeys.Add(newKey);
}

bool APD_Character::TryAddHealth(float healthValue)
{
	return HealthComponent->HealHealth(healthValue);
}

bool APD_Character::HasKey(FName keyTag){
    return DoorKeys.Contains(keyTag);
}

void APD_Character::SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

void APD_Character::SetMeleeState(bool NewState)
{
	bIsHittingMelee = NewState;
	bCanUseWeapon = !NewState;
}

void APD_Character::SetComboEnable(bool NewState)
{
	bIsComboEnable = NewState;
}

void APD_Character::ResetCombo()
{
	SetComboEnable(false);
	CurrentComboMultiplier = 1.0f;
}

void APD_Character::BeginBurnState(float NewBurnDamage)
{
	bIsBurning = true;
	BurnDamage = NewBurnDamage;

	if(IsValid(BurnEffect))
	{
		BurnEffectComponent = UGameplayStatics::SpawnEmitterAttached(BurnEffect, GetMesh(), BurnStatusSocketName);
	}

	if (IsValid(BurnSound))
	{
		BurnSoundComponent = UGameplayStatics::SpawnSoundAttached(BurnSound, GetMesh(), BurnStatusSocketName);
	}
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APD_Character::EndBurnState, 5, false);
}

void APD_Character::EndBurnState()
{
	bIsBurning = false; 
	BurnDamage = 0.0f;

	if (IsValid(BurnEffectComponent))
	{
		BurnEffectComponent->Deactivate();
	}

	if (IsValid(BurnSoundComponent))
	{
		BurnSoundComponent->Deactivate();
	}
}

void APD_Character::AddXPUltimateOverTime()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandleGainXPUltimateOverTimeBehavior, this, &APD_Character::GainUltimateXP, 1.0f, true);
}

void APD_Character::PlayStepSound()
{
	StepSoundComponent->Play();
}

void APD_Character::PlayVoiceSound(USoundCue * VoiceSound)
{
	if (!IsValid(VoiceSound))
	{
		return;
	}

	VoiceSoundComponent->SetSound(VoiceSound);
	VoiceSoundComponent->Play();
}

void APD_Character::GainUltimateXP()
{
	CurrentUltimateXP = FMath::Clamp(CurrentUltimateXP + 10, 0.0f, MaxUltimateXP);
	OnUltimateUpdateDelegate.Broadcast(CurrentUltimateXP, MaxUltimateXP);

	if (CurrentUltimateXP == MaxUltimateXP)
	{
		bCanUseUltimate = true;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleGainXPUltimateOverTimeBehavior);
		OnUltimateStatusDelegate.Broadcast(true);
	}

	BP_GainUltimateXP(10);

}

void APD_Character::GainUltimateXPValue(float XPValue)
{
	CurrentUltimateXP = FMath::Clamp(CurrentUltimateXP + XPValue, 0.0f, MaxUltimateXP);
	OnUltimateUpdateDelegate.Broadcast(CurrentUltimateXP, MaxUltimateXP);

	if (CurrentUltimateXP == MaxUltimateXP)
	{
		bCanUseUltimate = true;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleGainXPUltimateOverTimeBehavior);
		OnUltimateStatusDelegate.Broadcast(true);
	}

	BP_GainUltimateXP(XPValue);

}

#pragma region "Class Ultimate"

void APD_Character::StartUltimate()
{
	if (bCanUseUltimate && !bIsUsingUltimate)
	{
		CurrentUltimateDuration = MaxUltimateDuration;

		bCanUseUltimate = false;

		if (IsValid(MyAnimInstance) && IsValid(UltimateMontage))
		{
			const float StartUltimateMontageDuration = MyAnimInstance->Montage_Play(UltimateMontage);
			GetWorld()->GetTimerManager().SetTimer(TimerHandleBeginUltimateBehavior, this, &APD_Character::BeginUltimateBehavior, StartUltimateMontageDuration, false);
		}
		else
		{
			BeginUltimateBehavior();
		}

		BP_StartUltimate();
	}
}

void APD_Character::StopUltimate()
{
}

void APD_Character::UpdateUltimateDuration(float Value)
{
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration - Value, 0.0f, MaxUltimateDuration);
	BP_UpdateUltimateDuration(Value);

	if (CurrentUltimateDuration == 0.0f) 
	{
		bIsUsingUltimate = false;

		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		PlayRate = 1.0f;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleAutomaticShoot);

		if (!bUltimateWithTick)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandleUltimate);
		}

		BP_StopUltimate();
	}
}

void APD_Character::UpdateUltimateDurationWithTimer()
{
	UpdateUltimateDuration(UltimateFrequency);
}

void APD_Character::BeginUltimateBehavior()
{
	bIsUsingUltimate = true;
	GetCharacterMovement()->MaxWalkSpeed = UltimateWalkSpeed;
	PlayRate = UltimatePlayRate;

	if (!bUltimateWithTick)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandleUltimate, this, &APD_Character::UpdateUltimateDurationWithTimer, UltimateFrequency, true);
	}
}

#pragma endregion

#pragma region "Slow Time Ultimate"

void APD_Character::StartSlowTimeUltimate()
{
	if (bCanUseUltimate && !bIsUsingUltimate)
	{
		CurrentUltimateDuration = MaxUltimateDuration;

		bCanUseUltimate = false;
		OnUltimateStatusDelegate.Broadcast(false);

		PlayVoiceSound(UltimateSound);

		if (IsValid(MyAnimInstance) && IsValid(UltimateSlowTimeMontage))
		{
			const float StartUltimateMontageDuration = MyAnimInstance->Montage_Play(UltimateSlowTimeMontage);
			GetWorld()->GetTimerManager().SetTimer(TimerHandleBeginUltimateBehavior, this, &APD_Character::SlowTime, StartUltimateMontageDuration, false);
		}
		else
		{
			BeginUltimateBehavior();
		}
		BP_StartUltimateVisualEffect();
		BP_StartUltimate();
	}
}

void APD_Character::StopSlowTimeUltimate()
{
}

void APD_Character::SlowTime()
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(0.2f);
	CustomTimeDilation = 5.0f;
	BeginSlowTimeUltimateBehavior();
}

void APD_Character::UpdateSlowTimeUltimateDuration(float Value)
{
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration - Value, 0.0f, MaxUltimateDuration);
	OnUltimateUpdateDelegate.Broadcast(CurrentUltimateDuration, MaxUltimateDuration);
	BP_UpdateUltimateDuration(Value);

	if (CurrentUltimateDuration == 0.0f)
	{
		bIsUsingUltimate = false;
		OnUltimateStatusDelegate.Broadcast(false);
		CurrentUltimateXP = 0.0f;
		//AddXPUltimateOverTime();

		GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
		CustomTimeDilation = 1.0f;
		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		PlayRate = 1.0f;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleAutomaticShoot);

		if (!bUltimateWithTick)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandleUltimate);
		}

		BP_StopUltimate();
	}
}

void APD_Character::UpdateSlowTimeUltimateDurationWithTimer()
{
	UpdateSlowTimeUltimateDuration(SlowTimeUltimateMinusValue);
}

void APD_Character::BeginSlowTimeUltimateBehavior()
{
	bIsUsingUltimate = true;
	BP_StartUltimateSoundEffect();
	GetCharacterMovement()->MaxWalkSpeed = UltimateWalkSpeed;
	PlayRate = UltimatePlayRate;

	if (!bUltimateWithTick)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandleUltimate, this, &APD_Character::UpdateSlowTimeUltimateDurationWithTimer, SlowTimeUltimateFrequency, true);
	}
}

#pragma endregion

void APD_Character::GoToMainMenu()
{
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->SaveData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}

void APD_Character::GoToPauseMenu()
{
	APD_PlayerController* MyPlayerController = Cast<APD_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	WidgetPauseMenuInstance = CreateWidget<UPD_PauseMenuWidget>(GetWorld(), WidgetPauseMenu);
	WidgetPauseMenuInstance->AddToViewport();

	if (IsValid(MyPlayerController))
	{
		MyPlayerController->SetInputMode(FInputModeUIOnly());
		MyPlayerController->SetShowMouseCursor(true);
	}
}

//TODO - Implementación del metodo para interactuar con objetos
void APD_Character::Interact() {

}
