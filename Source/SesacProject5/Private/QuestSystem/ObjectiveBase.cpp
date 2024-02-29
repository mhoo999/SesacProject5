// copyright by LootingEmployment


#include "QuestSystem/ObjectiveBase.h"

AObjectiveBase::AObjectiveBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AObjectiveBase::BeginPlay()
{
	Super::BeginPlay();
}

void AObjectiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

