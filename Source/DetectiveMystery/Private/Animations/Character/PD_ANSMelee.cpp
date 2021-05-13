// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/PD_ANSMelee.h"
#include "PD_Character.h"
 
void UPD_ANSMelee::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) {
		APD_Character* Character = Cast<APD_Character>(CharacterActor);
		if (IsValid(Character)) {
			Character->SetMeleeDetectorCollision(ECollisionEnabled::QueryOnly);
		}
	}
}

void UPD_ANSMelee::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) {
		APD_Character* Character = Cast<APD_Character>(CharacterActor);
		if (IsValid(Character)) {
			Character->SetMeleeDetectorCollision(ECollisionEnabled::NoCollision);
		}
	}
}
