// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PD_Enemy.h"
#include "PD_Character.h"
#include "Weapon/PD_Rifle.h"
#include "Components/PD_HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/PD_Item.h"
#include "AIModule/Classes/Perception/AISense_Damage.h"
#include "Enemy/Controller/PD_AIController.h"
#include "Core/PD_GameInstance.h"
#include "Components/WidgetComponent.h"
#include "UI/Enemy/PD_EnemyHealthBar.h"

APD_Enemy::APD_Enemy()
{
	bLoopPath = false;
	DirectionIndex = 1;
	WaitingTimeOnPathPoint = 1.0f;
	XPValue = 20.0f;
	LootProbability = 100.0f;

	WidgetHealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealthBarComponent"));
	WidgetHealthBarComponent->SetupAttachment(RootComponent);
}

void APD_Enemy::BeginPlay()
{
	Super::BeginPlay();

	MyAIController = Cast<APD_AIController>(GetController());
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &APD_Enemy::HealthChanged);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &APD_Enemy::GiveXP);

	UUserWidget* WidgetObject = WidgetHealthBarComponent->GetUserWidgetObject();
	if(IsValid(WidgetObject))
	{
		EnemyHealthBar = Cast<UPD_EnemyHealthBar>(WidgetObject);
		if (IsValid(EnemyHealthBar))
		{
			HealthComponent->OnHealthUpdateDelegate.AddDynamic(EnemyHealthBar, &UPD_EnemyHealthBar::UpdateHealth);
			HideHealthBar();
		}
		
	}
}

void APD_Enemy::GiveXP(AActor* DamageCauser)
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
		}

	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BP_GiveXP(DamageCauser);
}

bool APD_Enemy::TrySpawnLoot()
{
	if (!IsValid(LootItemClass))
	{
		return false;
	}
	float SelectedProbability = FMath::RandRange(0.0f, 100.0f);
	if (SelectedProbability <= LootProbability)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<APD_Item>(LootItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameter);
	}

	return true;
}

void APD_Enemy::HealthChanged(UPD_HealthComponent * CurrentHealthComponent, AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (!IsValid(MyAIController))
	{
		return;
	}

	ShowHealthBar();
	if(bIsShowingHealthBar)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HideHealthBar);
	}
	else
	{
		ShowHealthBar();
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HideHealthBar, this, &APD_Enemy::HideHealthBar, 1.5f, false);

	if (CurrentHealthComponent->IsDead())
	{
		MyAIController->UnPossess();

		if (IsValid(GameInstanceReference))
		{
			GameInstanceReference->AddEnemyDefeatedToCounter();
		}
		HideHealthBar();
	}
	else
	{
		APD_Rifle* Rifle = Cast<APD_Rifle>(DamageCauser);
		if (IsValid(Rifle))
		{
			AActor* RifleOwner = Rifle->GetOwner();
			MyAIController->SetReceiveDamage(true);
			UAISense_Damage::ReportDamageEvent(GetWorld(), this, RifleOwner, Damage, RifleOwner->GetActorLocation(), FVector::ZeroVector);
		}
	}

}

void APD_Enemy::ShowHealthBar()
{
	bIsShowingHealthBar = true;
	EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
}


void APD_Enemy::HideHealthBar()
{
	bIsShowingHealthBar = false;
	EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
}
