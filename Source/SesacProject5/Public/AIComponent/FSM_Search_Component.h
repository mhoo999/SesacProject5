// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "FSM_Component.h"
#include "Components/ActorComponent.h"
#include "Interface/FSMInterface.h"
#include "FSM_Search_Component.generated.h"


class AHoonsAIController;
class ACharacterBase;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UFSM_Search_Component : public UFSM_Component, public IFSMInterface
{
	GENERATED_BODY()

public:	
	UFSM_Search_Component();

protected:
	virtual void BeginPlay() override;

public:
	virtual void ExecuteBehavior() override;
	virtual void StopExecute() override;
	virtual void SenseNewActor(AActor* NewActor) override;

private:
	bool bFind;
	
	UPROPERTY()
	FTimerHandle returnHandle;

	UFUNCTION()
	void AIReturnFunc();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings", meta=(AllowPrivateAccess))
	float retrunDuration = 10.f;

	bool bCuriosityMumble;

};
