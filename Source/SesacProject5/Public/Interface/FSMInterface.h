// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FSMInterface.generated.h"

class AAIController;
class APawn;

UINTERFACE(MinimalAPI)
class UFSMInterface : public UInterface
{
	GENERATED_BODY()
};

class SESACPROJECT5_API IFSMInterface
{
	GENERATED_BODY()

public:
	virtual void ExecutePatrol()=0;
	virtual void ExecuteSearch()=0;
	virtual void ExecuteChase()=0;
	virtual void ExecuteAttack()=0;
	virtual void ExecuteRetreatFiring()=0;
	virtual void ExecuteAdvanceFiring()=0;
	virtual void ExecuteEvade()=0;
	virtual void ExecuteCamping()=0;
	virtual void ExecuteSelfHealing()=0;
};
