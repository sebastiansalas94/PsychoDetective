// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PD_HealerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/PD_Enemy.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Components/PD_HealthComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "../DetectiveMystery.h"


// Sets default values
APD_HealerBot::APD_HealerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealerBotMeshComponent"));
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
	ExplosionDamage = 100.0f;
	ExplosionRadius = 50.0f;
	HealValue = 20.0f;
	bIsExploded = false;

}

// Called when the game starts or when spawned
void APD_HealerBot::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APD_Enemy::StaticClass(), FoundActors);

	AActor* EnemyPawn = FoundActors[0]; //UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(EnemyPawn))
	{
		EnemyCharacter = Cast<APD_Enemy>(EnemyPawn);
	}

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &APD_HealerBot::TakingDamage);
	SelfDestructionDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &APD_HealerBot::Heal);

	BotMaterial = BotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BotMeshComponent->GetMaterial(0));

	NextPathPoint = GetNextPathPoint();
	
}

// Called every frame
void APD_HealerBot::Tick(float DeltaTime)
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
		DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15, FColor::Green, false, 0.0f, 0, 1.0f);
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APD_Enemy::StaticClass(), FoundActors);
	bool bFoundEnemyToCure = false;
	
	/*for (int i = 0; i < FoundActors.Num() && !bFoundEnemyToCure; i++)
	{

		if (bDebug) 
		{
			UE_LOG(LogTemp, Verbose, TEXT("index: %f"), i);
		}

		APD_Enemy* EnemyCharacterToHeal = Cast<APD_Enemy>(FoundActors[i]);

		UPD_HealthComponent* EnemyHealthComponent = EnemyCharacterToHeal->GetHealthComponent();

		if (IsValid(EnemyHealthComponent) && !(EnemyHealthComponent->GetMaxHealth() == EnemyHealthComponent->GetCurrentHealth()))
		{
			bFoundEnemyToCure = true;
			Heal(EnemyCharacterToHeal);
		}
	}
	*/

}

FVector APD_HealerBot::GetNextPathPoint()
{
	if (!IsValid(EnemyCharacter))
	{
		return GetActorLocation();
	}

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), EnemyCharacter);

	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1];
	}

	//If Navigation points are less or equals than 1
	return GetActorLocation();
}


void APD_HealerBot::TakingDamage(UPD_HealthComponent* CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
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

void APD_HealerBot::SelfDestruction()
{
	if (bIsExploded)
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

	Destroy();
}

void APD_HealerBot::SelfDamage()
{
	UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), nullptr, nullptr);
}

void APD_HealerBot::Heal(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsValid(EnemyCharacter))
	{
		float EnemyMaxHealth = EnemyCharacter->GetHealthComponent()->GetMaxHealth();
		float EnemyCurrentHealth = EnemyCharacter->GetHealthComponent()->GetCurrentHealth();

		if (EnemyCurrentHealth < EnemyMaxHealth)
		{
			EnemyCharacter->GetHealthComponent()->HealHealth(HealValue);

			if (IsValid(HealEffect))
			{
				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(this);
				QueryParams.AddIgnoredActor(EnemyCharacter);
				QueryParams.bTraceComplex = true;

				FVector TraceEnd = FVector(0, 0, -1000);

				FHitResult HitResult;
				bool bHitGround = GetWorld()->LineTraceSingleByChannel(HitResult, EnemyCharacter->GetActorLocation(), TraceEnd, COLLISION_WEAPON, QueryParams);

				FVector AnimLocation = FVector(EnemyCharacter->GetActorLocation().X, EnemyCharacter->GetActorLocation().Y, HitResult.ImpactPoint.Z);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealEffect, AnimLocation);
			}
		}
		else if(!(EnemyCharacter->GetHealthComponent()->GetDefenseUp()))
		{
			EnemyCharacter->GetHealthComponent()->SetDefenseUpBuff();
			if (IsValid(DefenseUpEffect)) {
				DefenseUpEffectComponent = UGameplayStatics::SpawnEmitterAttached(DefenseUpEffect, EnemyCharacter->GetMesh(), "head");
			}
			GetWorldTimerManager().SetTimer(TimerHandleBuff, this, &APD_HealerBot::EndDefenseUpBuff, 5, false);
		}

		
	}

}

void APD_HealerBot::EndDefenseUpBuff()
{
	EnemyCharacter->GetHealthComponent()->EndDefenseUpBuff();
	if (IsValid(DefenseUpEffectComponent))
	{
		DefenseUpEffectComponent->Deactivate();
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleBuff);
}