// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/FSMInterface.h"
#include "FSM_Attack_Component.generated.h"


class UWeaponComponent;
class AHoonsAIController;
class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UFSM_Attack_Component : public UActorComponent, public IFSMInterface
{
	GENERATED_BODY()

public:	
	UFSM_Attack_Component();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ExecuteBehavior() override;
	virtual void StopExecute() override;
	virtual void SenseNewActor(AActor* NewActor) override;

public:
	UPROPERTY(BlueprintReadWrite)
	ACharacterBase* ai;

	UPROPERTY(BlueprintReadWrite)
	AHoonsAIController* ac;

	AActor* target;

	UPROPERTY()
	UWeaponComponent* WeaponComp;
};
