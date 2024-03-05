// copyright by LootingEmployment


#include "QuestSystem/QuestLogComponent.h"

#include "Character/CharacterBase.h"
#include "Component/EquipmentComponent.h"
#include "Component/EscapeComponent.h"
#include "Component/HealthComponent.h"
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
	player->GetComponentByClass<UHealthComponent>()->OnIsDeadChanged.AddUObject(this, &UQuestLogComponent::ReleaseComplete);

	inventoryComp = player->GetComponentByClass<UEquipmentComponent>();
	inventoryComp->OnInventoryChanged.AddUObject(this, &UQuestLogComponent::OnInventoryChanged);
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

	// UE_LOG(LogTemp, Warning, TEXT("UQuestLogComponent::CompleteQuest : %hhd"), questData.isProgress);
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

void UQuestLogComponent::ReleaseComplete(bool bNewIsDead)
{
	if (bNewIsDead)
	{
		for (FQuestManagement& quest : questList)
		{
			for (FStageDetails& stage : quest.questDetails.stages)
			{
				for (FObjectiveDetails& objective : stage.Objectives)
				{
					objective.objectiveComplete = false;
					objective.objectiveProgress = 0;
				}
			}
		}
	}

	auto gameInstance = GetWorld()->GetGameInstance<UEFSGameInstance>();
	gameInstance->questData->SaveQuestLog(questList);
}

void UQuestLogComponent::OnInventoryChanged(const TArray<FStorage>& storageArray)
{
	int32 ComparisonCount = 0;

	for (FQuestManagement& quest : questList)
	{
		for (FStageDetails& stage : quest.questDetails.stages)
		{
			for (FObjectiveDetails& objective : stage.Objectives)
			{
				ComparisonCount = inventoryComp->GetItemCount(objective.objectiveID);
				UE_LOG(LogTemp, Warning, TEXT("UQuestLogComponent::OnInventoryChanged : %s, %d"), *objective.objectiveID, ComparisonCount);
				
				if (ComparisonCount == objective.Quantity)
				{
					objective.objectiveComplete = true;
					UE_LOG(LogTemp, Warning, TEXT("Quest Name : %s /n isComplete : %hhd"), *quest.questID.ToString(), quest.isCompleted);
					UE_LOG(LogTemp, Warning, TEXT("objective Name : %s /n isComplete : %hhd"), *objective.objectiveName.ToString(), objective.objectiveComplete);
				}
			}
		}
	}
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
				if (objective.objectiveID.Equals(objectiveID, ESearchCase::IgnoreCase))
				{
					objective.objectiveProgress += 1;

					if (objective.objectiveProgress == objective.Quantity)
					{
						objective.objectiveComplete = true;
						UE_LOG(LogTemp, Warning, TEXT("Quest Name : %s /n isComplete : %hhd"), *quest.questID.ToString(), quest.isCompleted);
						UE_LOG(LogTemp, Warning, TEXT("objective Name : %s /n isComplete : %hhd"), *objective.objectiveName.ToString(), objective.objectiveComplete);
					}
				}
			}
		}
	}
}
