// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "FSM_Component.h"
#include "Components/ActorComponent.h"
#include "Interface/FSMInterface.h"
#include "FSM_Patrol_Component.generated.h"

class ACharacterBase;
class AHoonsAIController;
class AAIPatrolWaypoint;
class AAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UFSM_Patrol_Component : public UFSM_Component, public IFSMInterface
{
	GENERATED_BODY()

public:	
	UFSM_Patrol_Component();

protected:
	virtual void BeginPlay() override;

public:
	virtual void ExecuteBehavior() override;
	virtual void StopExecute() override;
	virtual void SenseNewActor(AActor* NewActor) override;

	int32 CurrentWaypointIndex = 0;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float AcceptanceRadius = 1.0f;
};
