// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PD_BotSpawner.h"
#include "Components/BillboardComponent.h"
#include "Enemy/PD_Bot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APD_BotSpawner::APD_BotSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpawnerBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("PathBillboard"));
	RootComponent = SpawnerBillboardComponent;

	bIsActive = true;
	MaxBotsCounter = 1;
	TimeToSpawn = 1.0f;

}

// Called when the game starts or when spawned
void APD_BotSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBot, this, &APD_BotSpawner::SpawnBot, TimeToSpawn, true);
	
}

void APD_BotSpawner::SpawnBot()
{
	if (!bIsActive)
	{
		return;
	}

	if (CurrentBotsCounter >= MaxBotsCounter)
	{
		return;
	}

	if (bIsActive)
	{
		if (IsValid(BotClass))
		{
			FVector LocalSpawnPoint = GetSpawnPoint();
			FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), LocalSpawnPoint);
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform BotTransform = FTransform(FRotator::ZeroRotator, SpawnPoint);

			//FVector CreateBotAnimationVector = SpawnPoint;
			//CreateBotAnimationVector.Set(SpawnPoint.X, SpawnPoint.Y + 400.0f, SpawnPoint.Z);
			//FTransform CreateBotAnimationTransform = FTransform(FRotator::ZeroRotator, CreateBotAnimationVector);

			APD_Bot* NewBot = GetWorld()->SpawnActorDeferred<APD_Bot>(BotClass, BotTransform);

			if (IsValid(NewBot))
			{
				NewBot->SetSpawner(this);
			}

			if(IsValid(CreateBotEffect))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CreateBotEffect, BotTransform);
			}
			NewBot->FinishSpawning(BotTransform);

			CurrentBotsCounter++;
		}
	}
}

FVector APD_BotSpawner::GetSpawnPoint()
{
	if (SpawnPoints.Num() > 0)
	{
		int IndexSelected = FMath::RandRange(0, SpawnPoints.Num() - 1);
		return SpawnPoints[IndexSelected];
	}
	else
	{
		return GetActorLocation();
	}
}

void APD_BotSpawner::NotifyBotDead()
{
	CurrentBotsCounter--;
}

