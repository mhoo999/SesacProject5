// copyright by LootingEmployment


#include "QuestSystem/QuestGiver.h"

AQuestGiver::AQuestGiver()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AQuestGiver::BeginPlay()
{
	Super::BeginPlay();
}

void AQuestGiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuestGiver::DisplayQuest()
{
}

