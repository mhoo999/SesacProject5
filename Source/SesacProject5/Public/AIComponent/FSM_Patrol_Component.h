// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "FSM_Component.h"
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

	bool IsAtDestination();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCPerformLookAround(UAnimMontage* MontageToPlay);

	void SetNextDestination();

	void MoveToDestination();

public:
	virtual void ExecuteBehavior() override;
	virtual void StopExecute() override;
	virtual void SenseNewActor(AActor* NewActor) override;

	int32 CurrentWaypointIndex = 0;
	AActor* NextWaypoint;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float AcceptanceRadius = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	UAnimMontage* lookAroundMontage;

	bool bHasPerformedLookAround = false;
	bool bHasNextWaypoint = false;

	FTimerHandle LookAroundTimerhandle;
	UFUNCTION()
	void OnLookAtroundTimerExpired();
};
