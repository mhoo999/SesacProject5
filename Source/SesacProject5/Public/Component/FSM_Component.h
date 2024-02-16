// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSM_Component.generated.h"


class UWeaponComponent;
class AHoonsAIController;
class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UFSM_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFSM_Component();

protected:
	virtual void BeginPlay() override;

	bool bFocusTarget() const;
	
public:
	UPROPERTY(BlueprintReadWrite)
	ACharacterBase* ai;

	UPROPERTY(BlueprintReadWrite)
	AHoonsAIController* ac;

	UPROPERTY()
	UWeaponComponent* WeaponComp;
	
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<AActor*> waypointArray;

	AActor* target;

	FVector targetLastLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sightRadius = 20.0f;
};
