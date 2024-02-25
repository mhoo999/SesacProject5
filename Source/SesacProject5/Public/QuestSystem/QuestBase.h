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
	// Sets default values for this actor's properties
	AQuestBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
