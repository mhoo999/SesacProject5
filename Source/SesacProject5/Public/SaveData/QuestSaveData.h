// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QuestSaveData.generated.h"

class UQuestLogComponent;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UQuestSaveData : public USaveGame
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem", meta=(AllowPrivateAccess))
	TArray<FQuestManagement> questList;
	
public:
	void PrintLog();
	
	UFUNCTION(BlueprintCallable)
	void SaveQuestLog(TArray<FQuestManagement> questListData);

	UFUNCTION(BlueprintCallable)
	void LoadQuestLog(UQuestLogComponent* questLogComp);	
};
