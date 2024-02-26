// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSM_Component.generated.h"


class AEOSAIController;
class UAIWeaponComponent;
class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UFSM_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFSM_Component();

	virtual void Init(APawn* pawn);

protected:
	virtual void BeginPlay() override;

	// bool bFocusTarget() const;

	void bAttacked(AActor* attacker);
	
	UPROPERTY(BlueprintReadWrite)
	ACharacterBase* ai;

	UPROPERTY(BlueprintReadWrite)
	AEOSAIController* ac;

	UPROPERTY()
	UAIWeaponComponent* WeaponComp;
	
	AActor* target;

	FVector targetLastLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sightRadius = 20.0f;
	
public:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<AActor*> waypointArray;
	
};
