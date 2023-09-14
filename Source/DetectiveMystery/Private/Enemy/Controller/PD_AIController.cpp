// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Controller/PD_AIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Enemy/PD_Enemy.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"

APD_AIController::APD_AIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	LoopPathParameterName = "bLoopPath";
	DirectionIndexParameterName = "DirectionIndex";
	WaitingTimeParameterName = "WaitingOnPathPoint";
	CanSeePlayerParameterName = "bCanSeePlayer";
	InvestigatingParameterName = "bIsInvestigating";
	TargetLocationParameterName = "TargetLocation";
}

void APD_AIController::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(EnemyBehaviorTree))
	{
		RunBehaviorTree(EnemyBehaviorTree);
	}

	MyEnemy = Cast<APD_Enemy>(K2_GetPawn());
	if (IsValid(MyEnemy))
	{
		MyBlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(this);

		if (IsValid(MyBlackBoard))
		{
			MyBlackBoard->SetValueAsBool(LoopPathParameterName, MyEnemy->GetLoopPath());
			MyBlackBoard->SetValueAsInt(DirectionIndexParameterName, MyEnemy->GetDirectionIndex());
			MyBlackBoard->SetValueAsFloat(WaitingTimeParameterName, MyEnemy->GetWaitingTime());
		}
	}

	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &APD_AIController::UpdateSenses);
}

void APD_AIController::UpdateSenses(const TArray<AActor*>& UpdatedActors)
{
	if (!IsValid(MyBlackBoard))
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		AIPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);

		APD_Character* SensedCharacter = Cast<APD_Character>(Actor);

		if (IsValid(SensedCharacter) && SensedCharacter->GetCharacterType() == EPD_CharacterType::CharacterType_Player)
		{
			for (int i = 0; i < PerceptionInfo.LastSensedStimuli.Num(); i++)
			{
				switch (i)
				{
				case 0:
					MyBlackBoard->SetValueAsBool(CanSeePlayerParameterName, PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed());

					if (IsValid(MyEnemy))
					{
						MyEnemy->SetAlert(PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed());
					}

					break;
				case 1:
					MyBlackBoard->SetValueAsBool(InvestigatingParameterName, bReceivingDamage);
					if (bReceivingDamage)
					{
						MyBlackBoard->SetValueAsVector(TargetLocationParameterName, PerceptionInfo.LastSensedStimuli[i].StimulusLocation);
					}
					break;
				default:
					break;
				}
			}
		}
	}

}

void APD_AIController::DeactivateAIPerception()
{
	AIPerceptionComponent->Deactivate();
}
