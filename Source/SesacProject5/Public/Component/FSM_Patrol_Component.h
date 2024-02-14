// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/FSMInterface.h"
#include "FSM_Patrol_Component.generated.h"

class AAIPatrolWaypoint;
class AAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UFSM_Patrol_Component : public UActorComponent, public IFSMInterface
{
	GENERATED_BODY()

public:	
	UFSM_Patrol_Component();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void ExecutePatrol() override;
	virtual void ExecuteSearch() override;
	virtual void ExecuteChase() override;
	virtual void ExecuteAttack() override;
	virtual void ExecuteRetreatFiring() override;
	virtual void ExecuteAdvanceFiring() override;
	virtual void ExecuteEvade() override;
	virtual void ExecuteCamping() override;
	virtual void ExecuteSelfHealing() override;
	
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<AActor*> waypointArray;

	int32 CurrentWaypointIndex = 0;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float AcceptanceRadius = 1.0f;
};
