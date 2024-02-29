// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestLocationBase.generated.h"

class UBoxComponent;

UCLASS()
class SESACPROJECT5_API AQuestLocationBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestLocationBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectDetail", meta=(AllowPrivateAccess))
	FString objectID = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectDetail", meta=(AllowPrivateAccess))
	int32 value = NULL;
};
