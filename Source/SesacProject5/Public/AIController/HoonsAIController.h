// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HoonsAIController.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API AHoonsAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
};
