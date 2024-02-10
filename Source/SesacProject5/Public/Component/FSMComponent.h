// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/FSMInterface.h"
#include "FSMComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UFSMComponent : public UActorComponent, public IFSMInterface
{
	GENERATED_BODY()

public:	
	UFSMComponent();

	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void ExecuteBehavior() override;
};
