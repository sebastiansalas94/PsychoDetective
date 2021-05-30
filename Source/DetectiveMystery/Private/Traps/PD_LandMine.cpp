// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/PD_LandMine.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../../DetectiveMystery.h"
#include "PD_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PD_HealthComponent.h"

// Sets default values
APD_LandMine::APD_LandMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LandMineDamage = 40.0f;
	MaxHealth = 10.0f;

	MainColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainColliderComponent"));
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = MainColliderComponent;

	LandMineMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LandMineMeshComponent"));
	LandMineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LandMineMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	LandMineMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	LandMineMeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	LandMineMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LandMineMeshComponent->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
	LandMineMeshComponent->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UPD_HealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void APD_LandMine::BeginPlay()
{
	Super::BeginPlay();

	MainColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &APD_LandMine::BeginAlert);
	MainColliderComponent->OnComponentEndOverlap.AddDynamic(this, &APD_LandMine::StopAlert);

	LandMineMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &APD_LandMine::ExplodeLandMine);
	
	HealthComponent->SetMaxHealth(MaxHealth);
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &APD_LandMine::OnHealthChange);
}

// Called every frame
void APD_LandMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_LandMine::BeginAlert(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsValid(OtherActor))
	{
		APD_Character* Character = Cast<APD_Character>(OtherActor);
		if (IsValid(Character)) {
			BP_MakeAlertSound();
		}
	}
}

void APD_LandMine::StopAlert(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		APD_Character* Character = Cast<APD_Character>(OtherActor);
		if (IsValid(Character)) {
			BP_StopAlertSound();
		}
	}
}

void APD_LandMine::ExplodeLandMine(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	BP_StopAlertSound();
	if (IsValid(OtherActor))
	{
		APD_Character* Character = Cast<APD_Character>(OtherActor);
		if (IsValid(Character)) {
			UGameplayStatics::ApplyRadialDamage(GetWorld(), LandMineDamage, GetActorLocation(), MainColliderComponent->GetScaledSphereRadius(), nullptr, TArray<AActor*>(), this, nullptr, true);
			DestroyLandMine();
		}
	}
}

void APD_LandMine::OnHealthChange(UPD_HealthComponent * CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (HealthComponent->IsDead())
	{
		DestroyLandMine();
	}
}

void APD_LandMine::DestroyLandMine()
{
	BP_MakeExplodeEffects();
	Destroy();
}

