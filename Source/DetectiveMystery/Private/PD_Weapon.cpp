// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_Weapon.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APD_Weapon::APD_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20.0f;
}

// Called when the game starts or when spawned
void APD_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_Weapon::StartAction() 
{
	BP_StartAction();
	PlaySound(ShotSound);
}

void APD_Weapon::StopAction()
{
	BP_StopAction();
}

void APD_Weapon::StartSecondaryAction()
{
	BP_StartSecondaryAction();
}

void APD_Weapon::StopSecondaryAction()
{
	BP_StopSecondaryAction();
}

void APD_Weapon::SetCharacterOwner(ACharacter* NewOwner) 
{
	if (IsValid(NewOwner)) {
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}
}

void APD_Weapon::PlaySound(USoundCue* SoundCue, bool bIs3D, FVector SoundLocation)
{
	if (!IsValid(SoundCue)) 
	{
		return;
	}

	if (bIs3D)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCue, SoundLocation);
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
	}
}

