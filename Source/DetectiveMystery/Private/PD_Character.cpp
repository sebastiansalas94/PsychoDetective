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

// Sets default values
APD_Character::APD_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraSocketName = "SCK_Camera";
	MeleeSocketName = "SCK_Melee";
	bIsFirstPersonView = true;
	bCanUseWeapon = true;
	MaxSpeedWalk = 600;
	MaxSpeedSprint = 1000;
	MeleeDamage = 10.0f;


	MaxComboMultiplier = 4;
	CurrentComboMultiplier = 1;


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
}

void APD_Character::InitiliazeReferences() {
	if (IsValid(GetMesh())) {
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	GameModeReference = Cast<APD_GameMode>(GetWorld()->GetAuthGameMode());
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
	}
}

void APD_Character::StopWeaponAction() {

	if (!bCanUseWeapon) {
		return;
	}
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StopAction();
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

	UE_LOG(LogTemp, Warning, TEXT("Entra al Melee"))

	if (bIsHittingMelee && !bCanMakeCombos) {
		return;
	}

	if (bCanMakeCombos) {

		if (bIsHittingMelee) {

			if (bIsComboEnable) {
				if (CurrentComboMultiplier < MaxComboMultiplier) {
					UE_LOG(LogTemp, Warning, TEXT("Se está ejecutando el combo: %d"), CurrentComboMultiplier)
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
		MyAnimInstance->Montage_Play(MeleeMontage);
	}
	SetMeleeState(true);
}

void APD_Character::StopMeleeAction() {
	UE_LOG(LogTemp, Warning, TEXT("Stop melee action"));
}

void APD_Character::MakeMeleeDamage(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsValid(OtherActor)) {
		UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
	}
}

void APD_Character::OnHealthChange(UPD_HealthComponent * CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (HealthComponent->IsDead()) 
	{
		if (IsValid(GameModeReference)) 
		{
			GameModeReference->GameOver(this);
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


}

void APD_Character::AddKey(FName newKey){
    DoorKeys.Add(newKey);
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

//TODO - Implementación del metodo para interactuar con objetos
void APD_Character::Interact() {

}
