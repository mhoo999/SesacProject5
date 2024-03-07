// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "FSM_Component.h"
#include "Components/ActorComponent.h"
#include "Interface/FSMInterface.h"
#include "FSM_Chase_Component.generated.h"

class IWeaponInterface;
class UWeaponComponent;
class ACharacterBase;
class AHoonsAIController;

struct TargetPart
{
	std::string name;
	int priority;
	bool isVisible;
	FVector partLoc;

	bool operator<(const TargetPart& other) const
	{
		return priority > other.priority;
	}
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UFSM_Chase_Component : public UFSM_Component, public IFSMInterface
{
	GENERATED_BODY()

public:	
	UFSM_Chase_Component();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void ExecuteBehavior() override;
	virtual void StopExecute() override;
	virtual void SenseNewActor(AActor* NewActor) override;

	bool bIsAttacking;
	
	bool FocusTargetPart(AActor* targetActor, FVector& TargetLocation);

private:
	bool bEnemyMumble;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float missingDuration = 5.f;

	bool bFind;

	UPROPERTY()
	FTimerHandle returnHandle;

	UFUNCTION()
	void AIReturnFunc();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings", meta=(AllowPrivateAccess))
	float retrunDuration = 15.f;
	
};
