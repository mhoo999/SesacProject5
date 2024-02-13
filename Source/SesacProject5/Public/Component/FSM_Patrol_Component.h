// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/FSMInterface.h"
#include "FSM_Patrol_Component.generated.h"


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
	virtual void ExecuteBehavior(AAIController* aiController, APawn* ai) override;

	bool UpdateRandomLocation(FVector origin, float radius, FVector& outLocation) const;

	FVector RandomLocation;
};
