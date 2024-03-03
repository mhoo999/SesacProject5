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
	UPROPERTY(EditAnywhere, Category="MySettings", Meta = (AllowPrivateAccess))
	TArray<AActor*> waypointArray;

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<AScavBase> ScavFactory;

	void MakeScave();

	void RespawnScave(bool bNewIsDead);

	FTimerHandle respawnTimer;

	UPROPERTY(EditAnywhere, Category="MySettings", Meta = (AllowPrivateAccess))
	float respawnTime = 30.f;
};
