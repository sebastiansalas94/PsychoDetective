// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PD_PortalGun.h"
#include "Weapon/PD_Portal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

APD_PortalGun::APD_PortalGun() {
	TraceLenght = 10000.0f;
	MuzzleSocketName = "SCK_Muzzle";
}

void APD_PortalGun::StartAction() {
	Super::StartAction();

	ShootPortal(true);

}

void APD_PortalGun::StopAction() {
	Super::StopAction();
}

void APD_PortalGun::StartSecondaryAction() {
	Super::StartSecondaryAction();

	ShootPortal(false);
}

void APD_PortalGun::StopSecondaryAction() {
	Super::StopSecondaryAction();
}

void APD_PortalGun::ShootPortal(bool bIsPrimaryPortal){

	AActor* CurrentOwner = GetOwner();

	if (IsValid(CurrentOwner))
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * TraceLenght);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		if (IsValid(CurrentPortal)) {
			QueryParams.AddIgnoredActor(CurrentPortal);
		}
		if (IsValid(CurrentPortal2)) {
			QueryParams.AddIgnoredActor(CurrentPortal2);
		}
		QueryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEnd;

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);


		if (IsValid(TraceEffect)) {
			USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
			if (IsValid(CharacterMeshComponent)) {

				FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
				UParticleSystemComponent* TraceComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzleSocketLocation);

				if (IsValid(TraceComponent)) {
					TraceComponent->SetVectorParameter(TraceParamName, TraceEndPoint);
				}
			}
		}

		FRotator ActorRotation = HitResult.ImpactNormal.Rotation();
		ActorRotation.Yaw += 180;


		if (bIsPrimaryPortal) {
			if (!IsValid(CurrentPortal))
				CurrentPortal = GetWorld()->SpawnActor<APD_Portal>(PortalClass, HitResult.ImpactPoint, ActorRotation);
			else {
				CurrentPortal->Destroy();
				CurrentPortal = GetWorld()->SpawnActor<APD_Portal>(PortalClass, HitResult.ImpactPoint, ActorRotation);
			}

			if (IsValid(CurrentPortal2)) {
				CurrentPortal->AssignOtherPortal(CurrentPortal2);
				CurrentPortal2->AssignOtherPortal(CurrentPortal);
			}
		}
		else {
			if (!IsValid(CurrentPortal2))
				CurrentPortal2 = GetWorld()->SpawnActor<APD_Portal>(PortalClass, HitResult.ImpactPoint, ActorRotation);
			else {
				CurrentPortal2->Destroy();
				CurrentPortal2 = GetWorld()->SpawnActor<APD_Portal>(PortalClass, HitResult.ImpactPoint, ActorRotation);
			}

			if (IsValid(CurrentPortal)) {
				CurrentPortal->AssignOtherPortal(CurrentPortal2);
				CurrentPortal2->AssignOtherPortal(CurrentPortal);
			}
		}
	}
}
