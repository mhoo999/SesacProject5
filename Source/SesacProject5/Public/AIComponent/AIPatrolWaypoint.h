// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIPatrolWaypoint.generated.h"

UCLASS()
class SESACPROJECT5_API AAIPatrolWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIPatrolWaypoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category="Waypoint")
	UStaticMeshComponent* VisualComponent;
};
