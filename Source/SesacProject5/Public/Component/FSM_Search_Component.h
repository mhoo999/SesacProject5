// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/FSMInterface.h"
#include "FSM_Search_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UFSM_Search_Component : public UActorComponent, public IFSMInterface
{
	GENERATED_BODY()

public:	
	UFSM_Search_Component();

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
};
