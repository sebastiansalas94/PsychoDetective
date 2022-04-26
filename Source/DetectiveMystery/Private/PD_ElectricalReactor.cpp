// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_ElectricalReactor.h"
#include "PD_ElectricalDoor.h"
#include "PD_Character.h"
#include "Components/StaticMeshComponent.h"
#include "../DetectiveMystery.h"
#include "Core/PD_GameInstance.h"

// Sets default values
APD_ElectricalReactor::APD_ElectricalReactor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsWorking = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	ElecReactorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElectricalReactor"));
	ElecReactorComponent->SetupAttachment(CustomRootComponent);
	ElecReactorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ElecReactorComponent->SetCollisionObjectType(COLLISION_MELEE);
	ElecReactorComponent->SetCollisionResponseToChannel(COLLISION_MELEE, ECR_Overlap);
}

// Called when the game starts or when spawned
void APD_ElectricalReactor::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();

	TArray<int> IndexArray = GameInstanceReference->GetReactorDestroyedArray();
	if (IndexArray.Contains(ReactorIndex))
	{
		bIsWorking = false;
		BP_ChangeMaterialExploded();
	}
}

// Called every frame
void APD_ElectricalReactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APD_ElectricalReactor::InitializeReferences()
{
	GameInstanceReference = Cast<UPD_GameInstance>(GetWorld()->GetGameInstance());
}

void APD_ElectricalReactor::NotifyActorBeginOverlap(AActor* OtherActor) 
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bIsWorking)
	{
		if (IsValid(OtherActor)) {
			APD_Character* OverlappedCharacter = Cast<APD_Character>(OtherActor);
			if (IsValid(OverlappedCharacter) && IsValid(ElectricalDoor)) {
				bIsWorking = false;
				ExplodeReactor();
				ElectricalDoor->ValidateToOpenDoor();
			}
		}
	}
}

void APD_ElectricalReactor::ExplodeReactor() 
{

	GameInstanceReference->AddReactorDestroyedToCounter(ReactorIndex);

	BP_SpawnEffectExplosion();
	BP_ChangeMaterialExploded();
}

