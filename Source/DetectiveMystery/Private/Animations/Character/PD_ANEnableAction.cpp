// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/PD_ANEnableAction.h"
#include "PD_Character.h"

void UPD_ANEnableAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) {
		APD_Character* Character = Cast<APD_Character>(CharacterActor);
		if (IsValid(Character)) {
			Character->SetMeleeState(false);
		}
	}
}
