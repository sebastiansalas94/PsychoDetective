// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/PD_ExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "../../DetectiveMystery.h"
#include "PD_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PD_HealthComponent.h"
#include "Misc/App.h"
#include "Components/SphereComponent.h"

// Sets default values
APD_ExplosiveBarrel::APD_ExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosiveBarrelDamage = 5.0f;

	ExplosiveBarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosiveBarrelMeshComponent"));
	ExplosiveBarrelMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ExplosiveBarrelMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExplosiveBarrelMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ExplosiveBarrelMeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ExplosiveBarrelMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ExplosiveBarrelMeshComponent->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
	RootComponent = ExplosiveBarrelMeshComponent;

	ExplosionColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionColliderComponent"));
	ExplosionColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ExplosionColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExplosionColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ExplosionColliderComponent->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UPD_HealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void APD_ExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &APD_ExplosiveBarrel::OnHealthChange);
}

// Called every frame
void APD_ExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_ExplosiveBarrel::OnHealthChange(UPD_HealthComponent * CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors);
	
	for (int i = 0; i < OverlappedActors.Num(); i++)
	{
		APD_Character* Character = Cast<APD_Character>(OverlappedActors[i]);
		if (IsValid(Character)) {
			Character->BeginBurnState(ExplosiveBarrelDamage);
		}
	}

	if (HealthComponent->IsDead())
	{
		DestroyExplosiveBarrel();
	}
}

void APD_ExplosiveBarrel::DestroyExplosiveBarrel()
{
	BP_MakeExplodeEffects();
	Destroy();
}
