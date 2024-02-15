// copyright by LootingEmployment


#include "Component/FSM_Chase_Component.h"

#include "AIController/HoonsAIController.h"
#include "Character/CharacterBase.h"
#include "Component/WeaponComponent.h"

UFSM_Chase_Component::UFSM_Chase_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Chase_Component::BeginPlay()
{
	Super::BeginPlay();

	ac = Cast<AHoonsAIController>(GetOwner());
	ai = Cast<ACharacterBase>(ac->GetPawn());

	WeaponComp = ai->GetComponentByClass<UWeaponComponent>();
}

void UFSM_Chase_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFSM_Chase_Component::ExecuteBehavior()
{
	// 발견한 플레이어에게 이동
	ac->MoveToActor(target, 10.f, true, true, false, 0, true);
	
	FVector AILocation = ai->GetActorLocation();
	FVector PlayerLocation = target->GetActorLocation();

	FHitResult hitResult;
	bool bCanSeePlayer = GetWorld()->LineTraceSingleByChannel(hitResult, AILocation, PlayerLocation, ECC_Visibility);

	// 시야에 보인다면 공격 거리 까지 이동
	if (bCanSeePlayer && hitResult.GetActor() == target)
	{
		float dist = FVector::Dist(target->GetActorLocation(), ai->GetActorLocation());
		float attackDist = WeaponComp->GetWeaponAttackRange();

		// 공격 거리 내라면 공격
		if (dist <= attackDist)
		{
			ac->StopMovement();
			ac->SetContext(EEnemystate::attack);
			ac->GetFSM()->SenseNewActor(target);
		}
	}
	// 보이지 않는다면 Search 상태로 변경
	else
	{
		ac->SetContext(EEnemystate::search);
	}
}

void UFSM_Chase_Component::StopExecute()
{
	ac->StopMovement();
}

void UFSM_Chase_Component::SenseNewActor(AActor* NewActor)
{
	if (NewActor == nullptr)
	{
		target = nullptr;
		ac->SetContext(EEnemystate::patrol);
	}
	else
	{
		target = NewActor;
	}
}

