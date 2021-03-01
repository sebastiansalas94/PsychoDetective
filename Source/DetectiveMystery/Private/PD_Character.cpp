// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_Character.h"

// Sets default values
APD_Character::APD_Character()
{
     // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_Character::BeginPlay()
{
    Super::BeginPlay();
    
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

}
