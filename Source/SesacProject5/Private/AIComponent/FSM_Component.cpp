// copyright by LootingEmployment


#include "AIComponent/FSM_Component.h"

#include "AIComponent/AIWeaponComponent.h"
#include "AIController/EOSAIController.h"
#include "Character/CharacterBase.h"
#include "Interface/FSMInterface.h"

UFSM_Component::UFSM_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Component::Init(APawn* pawn)
{
	ai = Cast<ACharacterBase>(pawn);
	// UE_LOG(LogTemp,	Warning, TEXT("UFSM_Component::Init) AI : %s"), *ai->GetActorNameOrLabel());
	WeaponComp = ai->GetComponentByClass<UAIWeaponComponent>();
}

void UFSM_Component::BeginPlay()
{
	Super::BeginPlay();

	ac = Cast<AEOSAIController>(GetOwner());
	SetIsReplicated(true);
}

bool UFSM_Component::bFocusTarget()
{
	if (target == nullptr)
	{
		return false;
	}
	
	FHitResult hitResult;
	FVector AILocation = ai->GetActorLocation();
	FVector TargetLocation = target->GetActorLocation();
	GetWorld()->LineTraceSingleByChannel(hitResult, AILocation, TargetLocation, ECC_Visibility);
	// DrawDebugLine(GetWorld(), AIEyeLocation, PlayerPartLocation, FColor::Magenta, false, 0.1f, 0, 10.0f);
	
	if (hitResult.GetActor() == target)
	{
		return true;
	}
	
	return false;
}

void UFSM_Component::bAttacked(AActor* attacker)
{
	ac->GetFSM()->StopExecute();
	ac->SetContext(EEnemystate::chase);
	ac->GetFSM()->SenseNewActor(attacker);
}

