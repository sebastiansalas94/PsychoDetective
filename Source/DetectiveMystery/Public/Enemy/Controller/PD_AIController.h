// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PD_AIController.generated.h"

class APD_Enemy;
class UBehaviorTree;
class UBlackboardComponent;
class UAIPercetionComponent;
/**
 * 
 */
UCLASS()
class DETECTIVEMYSTERY_API APD_AIController : public AAIController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionComponent* AIPerceptionComponent;

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Controller")
	bool bReceivingDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName LoopPathParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName DirectionIndexParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName WaitingTimeParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName CanSeePlayerParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName InvestigatingParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName TargetLocationParameterName;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Controller")
	APD_Enemy* MyEnemy;
	
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Controller")
	UBlackboardComponent* MyBlackBoard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	UBehaviorTree* EnemyBehaviorTree;

public:
	// Sets default values for this character's properties
	APD_AIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateSenses(const TArray<AActor*>& UpdatedActors);

public:
	void SetReceiveDamage(bool newState) { bReceivingDamage = newState;  };
};
