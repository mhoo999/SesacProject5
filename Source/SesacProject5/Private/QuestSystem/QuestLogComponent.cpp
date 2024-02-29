// copyright by LootingEmployment


#include "QuestSystem/QuestLogComponent.h"

#include "Character/CharacterBase.h"
#include "Component/EscapeComponent.h"
#include "GameInstance/EFSGameInstance.h"
#include "SaveData/QuestSaveData.h"

UQuestLogComponent::UQuestLogComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuestLogComponent::BeginPlay()
{
	Super::BeginPlay();

	auto player = Cast<ACharacterBase>(GetOwner());
	auto escapeComp = player->GetComponentByClass<UEscapeComponent>();
	escapeComp->OnEscape.AddUObject(this, &UQuestLogComponent::saveQuest);
}

void UQuestLogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UQuestLogComponent::AddNewQuest(FName questID, FDataTableRowHandle questRow)
{
	if (HasQuest(questID))
	{
		return;
	}
	
	FQuestManagement NewQuest;
	NewQuest.questID = questID;

	AcceptQuest(NewQuest, questRow);

	questList.Add(NewQuest);
}

void UQuestLogComponent::CompleteQuest(FQuestManagement questData)
{
	questData.isProgress = false;
}

void UQuestLogComponent::QueryActiveQuest()
{
}

void UQuestLogComponent::TrackQuest()
{
}

bool UQuestLogComponent::HasQuest(FName questID)
{
	for (const FQuestManagement& quest : questList)
	{
		if (quest.questID == questID)
		{
			return true;
		}
	}
	
	return false;
}

void UQuestLogComponent::AcceptQuest(FQuestManagement& quest, FDataTableRowHandle& questRow)
{
	FQuestDetails* rowData = nullptr;
	if (questRow.DataTable != nullptr)
	{
		rowData = questRow.DataTable->FindRow<FQuestDetails>(questRow.RowName, TEXT("LookupQuestDetails"));
	}

	if (rowData != nullptr)
	{
		quest.questDetails = *rowData;
		quest.isCompleted = false;
		quest.isProgress = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest details not found in data table for quest ID: %s"), *questRow.RowName.ToString());
	}
}

void UQuestLogComponent::saveQuest()
{
	auto gameInstance = GetWorld()->GetGameInstance<UEFSGameInstance>();
	gameInstance->questData->SaveQuestLog(questList);
}

void UQuestLogComponent::ClearQuestList()
{
	questList.Empty();
}

void UQuestLogComponent::ClientRPCOnObjectiveIDCalled_Implementation(const FString& objectiveID, int32 value)
{
	for (FQuestManagement& quest : questList)
	{
		for (FStageDetails& stage : quest.questDetails.stages)
		{
			for (FObjectiveDetails& objective : stage.Objectives)
			{
				if (objective.objectiveID.Equals(objectiveID, ESearchCase::IgnoreCase) && objective.Quantity == value)
				{
					quest.isCompleted = true;
					// UE_LOG(LogTemp, Warning, TEXT("questID : %s is COMPLETE"), *quest.questID.ToString());
				}
			}
		}
	}
}
