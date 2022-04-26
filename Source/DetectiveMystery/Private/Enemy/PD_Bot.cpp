// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PD_Bot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PD_Character.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Components/PD_HealthComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/PD_HealthComponent.h"
#include "Weapon/PD_Rifle.h"
#include "Items/PD_Item.h"
#include "Enemy/PD_BotSpawner.h"
#include "Items/PD_BotSpawnerKey.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/PD_GameInstance.h"


// Sets default values
APD_Bot::APD_Bot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotMeshComponent"));
	BotMeshComponent->SetCanEverAffectNavigation(false);
	BotMeshComponent->SetSimulatePhysics(true);
	RootComponent = BotMeshComponent;

	HealthComponent = CreateDefaultSubobject<UPD_HealthComponent>(TEXT("HealthComponent"));

	SelfDestructionDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SelfDestructionDetector"));
	SelfDestructionDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SelfDestructionDetectorComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SelfDestructionDetectorComponent->SetSphereRadius(150.0f);
	SelfDestructionDetectorComponent->SetupAttachment(RootComponent);


	MinDistanceToTarget = 100.0f;
	ForceMagnitude = 500.0f;
	ExplosionDamage = 20.0f;
	ExplosionRadius = 50.0f;
	bIsExploded = false;
	XPValue = 20.0f;
	BotSpawnerKeyLootProbability = 25.0f;
	LootProbability = 100.0f - BotSpawnerKeyLootProbability;

}

// Called when the game starts or when spawned
void APD_Bot::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		PlayerCharacter = Cast<APD_Character>(PlayerPawn);
	}

	GameInstanceReference = Cast<UPD_GameInstance>(GetWorld()->GetGameInstance());
	
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &APD_Bot::TakingDamage);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &APD_Bot::GiveXP);
	SelfDestructionDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &APD_Bot::StartCountDown);

	BotMaterial = BotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BotMeshComponent->GetMaterial(0));

	NextPathPoint = GetNextPathPoint();

}

// Called every frame
void APD_Bot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
	if (DistanceToTarget <= MinDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= ForceMagnitude;

		BotMeshComponent->AddForce(ForceDirection, NAME_None, true);
	}
	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15, FColor::Purple, false, 0.0f, 0, 1.0f);
	}
}

FVector APD_Bot::GetNextPathPoint() 
{
	if(!IsValid(PlayerCharacter))
	{
		return GetActorLocation();
	}

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), PlayerCharacter);

	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1];
	}

	//If Navigation points are less or equals than 1
	return GetActorLocation();
}

void APD_Bot::TakingDamage(UPD_HealthComponent* CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (IsValid(BotMaterial))
	{
		BotMaterial->SetScalarParameterValue("Pulse", GetWorld()->TimeSeconds);
	}

	if (CurrentHealthComponent->IsDead())
	{
		SelfDestruction();
	}
}

void APD_Bot::SelfDestruction()
{
	if(bIsExploded)
	{
		return;
	}
	bIsExploded = true;

	if (IsValid(ExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}


	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 20, FColor::Red, true, 5.0f, 0, 2.0f);
	}

	if (IsValid(MySpawner))
	{
		MySpawner->NotifyBotDead();
	}

	Destroy();
}

void APD_Bot::StartCountDown(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (bIsStartingCountdown)
	{
		return;
	}

	if (OtherActor == PlayerCharacter)
	{
		bIsStartingCountdown = true;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SelfDamage, this, &APD_Bot::SelfDamage, 0.5f, true);
	}
}

void APD_Bot::SelfDamage()
{
	UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), nullptr, nullptr);
}

void APD_Bot::GiveXP(AActor* DamageCauser) 
{
	APD_Character* PossiblePlayer = Cast<APD_Character>(DamageCauser);
	if (IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == EPD_CharacterType::CharacterType_Player)
	{
		PossiblePlayer->GainUltimateXPValue(XPValue);
		TrySpawnLoot();
	}

	APD_Rifle* PossibleRifle = Cast<APD_Rifle>(DamageCauser);
	if (IsValid(PossibleRifle))
	{
		APD_Character* RifleOwner = Cast<APD_Character>(PossibleRifle->GetOwner());
		if (IsValid(RifleOwner) && RifleOwner->GetCharacterType() == EPD_CharacterType::CharacterType_Player)
		{
			RifleOwner->GainUltimateXPValue(XPValue);
			TrySpawnLoot();

			if (IsValid(GameInstanceReference))
			{
				GameInstanceReference->AddEnemyDefeatedToCounter();
			}
		}

	}

	BP_GiveXP(DamageCauser);
}

bool APD_Bot::TrySpawnLoot()
{
	if (!IsValid(LootItemClass))
	{
		return false;
	}

	if (!IsValid(BotSpawnerKeyLootItemClass))
	{
		return false;
	}

	float SelectedProbability = FMath::RandRange(0.0f, 100.0f);
	
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	bool bBotSpawnerIsActive = false;


	if (IsValid(MySpawner))
	{
		bBotSpawnerIsActive = MySpawner->GetIsActive();
	}

	if (bBotSpawnerIsActive && SelectedProbability <= BotSpawnerKeyLootProbability)
	{

		FTransform botT = GetActorTransform();
		FVector botV = GetActorLocation();

		FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), GetActorLocation());
		FTransform BotSpawnerKeyTransform = FTransform(FRotator::ZeroRotator, botV);

		APD_BotSpawnerKey* NewBotSpawnerKey = GetWorld()->SpawnActorDeferred<APD_BotSpawnerKey>(BotSpawnerKeyLootItemClass, BotSpawnerKeyTransform);
		if (IsValid(NewBotSpawnerKey))
		{
			NewBotSpawnerKey->SetSpawner(MySpawner);
		}
		NewBotSpawnerKey->FinishSpawning(BotSpawnerKeyTransform);
	}
	else if (LootProbability <= SelectedProbability)
	{
		GetWorld()->SpawnActor<APD_Item>(LootItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameter);
	}

	return true;
}