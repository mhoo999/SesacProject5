// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveBase.generated.h"

UCLASS()
class SESACPROJECT5_API AObjectiveBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AObjectiveBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectInfo", meta=(AllowPrivateAccess))
	FString objectID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectInfo", meta=(AllowPrivateAccess))
	int32 value;
};
