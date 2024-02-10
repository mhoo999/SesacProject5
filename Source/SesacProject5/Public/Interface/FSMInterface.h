// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FSMInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFSMInterface : public UInterface
{
	GENERATED_BODY()
};

class SESACPROJECT5_API IFSMInterface
{
	GENERATED_BODY()

public:
	virtual void ExecuteBehavior();
};
