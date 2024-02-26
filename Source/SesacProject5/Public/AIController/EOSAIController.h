// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EOSAIController.generated.h"

class UFSM_Component;
class UAISenseConfig_Hearing;
class UFSM_Attack_Component;
class UFSM_Chase_Component;
class UFSM_Search_Component;
class UAISenseConfig_Sight;
struct FAIStimulus;
class ACharacterBase;
class IFSMInterface;
class UFSM_Patrol_Component;
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FInitDelegate);

UENUM(BlueprintType)
enum class EEnemystate : uint8
{
	patrol				UMETA(DisplayName = "Patrol"),
	search				UMETA(DisplayName = "Search"),
	chase				UMETA(DisplayName = "Chase"),
	retreatFiring		UMETA(DisplayName = "Retreat Firing"),
	AdvanceFiring		UMETA(DisplayName = "Advance Firing"),
	evade				UMETA(DisplayName = "Evade"),
	camping				UMETA(DisplayName = "Camping"),
	selfHealing			UMETA(DisplayName = "Self Healing")
};

UCLASS()
class SESACPROJECT5_API AEOSAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEOSAIController();

	FInitDelegate InitDelegate;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	IFSMInterface* FSMInterface;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|FSM", meta=(AllowAbstract))
	UFSM_Patrol_Component* FSMPatrolComp;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|FSM", meta=(AllowAbstract))
	UFSM_Search_Component* FSMSearchComp;
	
	UPROPERTY(EditDefaultsOnly, Category="MySettings|FSM", meta=(AllowAbstract))
	UFSM_Chase_Component* FSMChaseComp;
	
	UPROPERTY(EditDefaultsOnly, Category="MySettings|AI", meta=(AllowAbstract))
	UAIPerceptionComponent* AIPerception;
	
	UPROPERTY(VisibleAnywhere, meta=(AllowAbstract))
	EEnemystate state;

	// A Sight Sense config for our AI
	UAISenseConfig_Sight* sight;
	UAISenseConfig_Hearing* hearing;

	void ChangeDead(bool bNewIsDead);

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	ACharacterBase* ai;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings|ETC", meta=(AllowPrivateAccess))
	float missTime = 10.f;

	UFUNCTION()
	void OnPerception(AActor* actor, FAIStimulus stimulus);

	virtual void OnPossess(APawn* InPawn) override;
	
	void printLog();
	
	void beAttacked(AActor* attacker);
	
public:
	IFSMInterface* GetFSM();
	
	void SetWaypoint(TArray<AActor*> waypointArray);
	
	void SetContext(EEnemystate next);
	
};
