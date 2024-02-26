// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestBase.generated.h"


// 퀘스트의 목표 유형
UENUM()
enum class EObjectiveType : uint8
{
	location UMETA(DisplayName = "Location"),
	kill UMETA(DisplayName = "Kill"),
	interact UMETA(DisplayName = "Interact"),
	collect UMETA(DisplayName = "Collect")
};

// 퀘스트 내 개별 세부 정보
USTRUCT(BlueprintType)
struct FObjectiveDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	FText objectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	EObjectiveType type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	FString objectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	int Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	bool isOptional;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	int BonusXP;
};

// 퀘스트의 단계별 세부 정보
USTRUCT(BlueprintType)
struct FStageDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	FText stageName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	FObjectiveDetails Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	int XRReward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	TMap<FName, int> itemReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	int currencyRewards;
};

// 전체 퀘스트의 세부 정보
USTRUCT(BlueprintType)
struct FQuestDetails : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	FText questName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	FText logDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	FText trackingDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	bool isMainQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem")
	TArray<FStageDetails> stages;
};

UCLASS()
class SESACPROJECT5_API AQuestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AQuestBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


private:
	// 퀘스트 데이터가 상주할 데이터 테이블의 행 이름을 참조
	FName questID;
	
	// 퀘스트 세부 정보
	FQuestDetails questDetails;
	
	// 퀘스트 단계
	int currentStage;
	
	// 현재 단계에 어떤 데이터가 있는지 추적
	FStageDetails currentStageDetails;
	
	// Objective, ObjectiveID를 추적
	TMap<FString, int> currentObjectiveProgress;
	
	// 완료 여부
	bool isCompleted;
	
};
