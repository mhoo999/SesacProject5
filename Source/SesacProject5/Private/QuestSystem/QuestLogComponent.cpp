// copyright by LootingEmployment


#include "QuestSystem/QuestLogComponent.h"

UQuestLogComponent::UQuestLogComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuestLogComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuestLogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UQuestLogComponent::AddNewQuest()
{
	
}

void UQuestLogComponent::CompleteQuest()
{
}

void UQuestLogComponent::QueryActiveQuest()
{
}

void UQuestLogComponent::TrackQuest()
{
}

