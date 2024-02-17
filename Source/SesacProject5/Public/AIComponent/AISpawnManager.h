// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnManager.generated.h"

class AScavBase;

UCLASS()
class SESACPROJECT5_API AAISpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AAISpawnManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	TArray<AActor*> waypointArray;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AScavBase> ScavFactory;

	void MakeScave();
};
