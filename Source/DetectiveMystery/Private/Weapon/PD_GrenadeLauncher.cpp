// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PD_GrenadeLauncher.h"
#include "Weapon/PD_Projectile.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

APD_GrenadeLauncher::APD_GrenadeLauncher() {
	MuzzleSocketName = "SCK_Muzzle";
}

void APD_GrenadeLauncher::StartAction() {
	Super::StartAction();

	if (IsValid(CurrentOwnerCharacter)) {
		USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
		if (IsValid(CharacterMeshComponent)) {

		FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleSocketRotation = CharacterMeshComponent->GetSocketRotation(MuzzleSocketName);

		APD_Projectile* CurrentProjectile = GetWorld()->SpawnActor<APD_Projectile>(ProjectileClass, MuzzleSocketLocation, MuzzleSocketRotation);
		}
	}
}

void APD_GrenadeLauncher::StopAction() {
	Super::StopAction();
}
