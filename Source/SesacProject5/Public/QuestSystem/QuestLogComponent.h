// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestLogComponent.generated.h"


class AQuestBase;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UQuestLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQuestLogComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 현재 진행중인 퀘스트 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem", meta=(AllowPrivateAccess))
	TArray<FName> currentActiveQuests;

	// 진행 완료한 퀘스트 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem", meta=(AllowPrivateAccess))
	TArray<FName> completedQuests;

	// 현재 추적중인 퀘스트 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem", meta=(AllowPrivateAccess))
	TArray<FName> currentTrackedQuest;

	// 수행중인 퀘스트 객체 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestSystem", meta=(AllowPrivateAccess))
	TArray<AQuestBase*> currentQuests;

public:
	// 새로운 퀘스트를 받는 함수
	UFUNCTION(BlueprintCallable)
	void AddNewQuest();
	
	// 퀘스트를 완료하여 보상 받는 함수
	UFUNCTION(BlueprintCallable)
	void CompleteQuest();
	
	// 활성화된 퀘스트에 쿼리 보내기
	UFUNCTION(BlueprintCallable)
	void QueryActiveQuest();
	
	// 퀘스트 추적
	UFUNCTION(BlueprintCallable)
	void TrackQuest();
};
