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
	virtual void ExecuteBehavior(AActor* target) override;
	virtual void StopExecute() override;
	virtual void SenseNewActor(AActor* NewActor) override;
};
