// copyright by LootingEmployment


#include "SaveData/QuestSaveData.h"

#include "GameInstance/EFSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/QuestLogComponent.h"

void UQuestSaveData::PrintLog()
{
	if (questList.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestSaveData::PrintLog) Quest List is empty"));
		return;
	}
	
	for (FQuestManagement Iter : questList)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestSaveData::PrintLog) Quest ID : %s"), *Iter.questID.ToString());
	}
}

void UQuestSaveData::SaveQuestLog(TArray<FQuestManagement> questListData)
{
	questList = questListData;
	UGameplayStatics::SaveGameToSlot(this, "questdata", 0);
}

void UQuestSaveData::LoadQuestLog(UQuestLogComponent* questLogComp)
{
	questLogComp->questList = questList;
}
