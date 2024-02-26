// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestGiver.generated.h"

UCLASS()
class SESACPROJECT5_API AQuestGiver : public AActor
{
	GENERATED_BODY()
	
public:	
	AQuestGiver();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	// QuestData에 있는 row data handle
	FDataTableRowHandle questData;

private:
	// Quest 노출 함수
	void DisplayQuest();
};
