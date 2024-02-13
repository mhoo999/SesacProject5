// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HoonsAIController.generated.h"

class IFSMInterface;
class UFSM_Patrol_Component;
/**
 * 
 */

UENUM(BlueprintType)
enum class EEnemystate : uint8
{
	patrol				UMETA(DisplayName = "Patrol"),
	search				UMETA(DisplayName = "Search"),
	chase				UMETA(DisplayName = "Chase"),
	attack				UMETA(DisplayName = "Attack"),
	retreatFiring		UMETA(DisplayName = "Retreat Firing"),
	AdvanceFiring		UMETA(DisplayName = "Advance Firing"),
	evade				UMETA(DisplayName = "Evade"),
	camping				UMETA(DisplayName = "Camping"),
	selfHealing			UMETA(DisplayName = "Self Healing")
};

UCLASS()
class SESACPROJECT5_API AHoonsAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHoonsAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	IFSMInterface* FSMInterface;
	
	UPROPERTY(EditDefaultsOnly, Category="MySettings|FSM", meta=(AllowAbstract))
	UFSM_Patrol_Component* FSMPatrolComp;

	UPROPERTY()
	APawn* ai;
};
