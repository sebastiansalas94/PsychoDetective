// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PD_Weapon.h"

// Sets default values
APD_Character::APD_Character()
{
     // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    FPSCameraSocketName = "SCK_Camera";
    bIsFirstPersonView = true;
    MaxSpeedWalk = 600;
    MaxSpeedSprint = 1000;
    
    FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
    FPSCameraComponent->bUsePawnControlRotation = true;
    FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);
    
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetupAttachment(RootComponent);
    
    TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
    TPSCameraComponent->SetupAttachment(SpringArmComponent);
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
	CreateInitialWeapon();
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

	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartAction();
	}
}

void APD_Character::StopWeaponAction() {

	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StopAction();
	}
}

void APD_Character::StartWeaponSecondaryAction() {

	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartSecondaryAction();
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

}

void APD_Character::AddKey(FName newKey){
    DoorKeys.Add(newKey);
}

bool APD_Character::HasKey(FName keyTag){
    return DoorKeys.Contains(keyTag);
}

//TODO - Implementación del metodo para interactuar con objetos
void APD_Character::Interact() {

}
