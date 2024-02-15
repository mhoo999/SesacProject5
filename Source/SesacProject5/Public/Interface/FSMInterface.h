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
	virtual void ExecuteBehavior(AActor* target) = 0;
	virtual void StopExecute() = 0;
	virtual void SenseNewActor(AActor* NewActor) = 0;
};
