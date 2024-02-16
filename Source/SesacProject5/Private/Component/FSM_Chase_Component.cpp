// copyright by LootingEmployment


#include "Component/FSM_Chase_Component.h"

#include "InputActionValue.h"
#include "AIController/EOSAIController.h"
#include "Character/CharacterBase.h"
#include "Component/WeaponComponent.h"

UFSM_Chase_Component::UFSM_Chase_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Chase_Component::BeginPlay()
{
	Super::BeginPlay();
}

void UFSM_Chase_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFSM_Chase_Component::ExecuteBehavior()
{
	// 시야에 타겟이 보인다면 공격
	if (bFocusTarget())
	{
		float dist = FVector::Dist(target->GetActorLocation(), ai->GetActorLocation());
		float attackDist = WeaponComp->GetWeaponAttackRange();
		
		if (dist <= attackDist)
		{
			ac->StopMovement();
			WeaponComp->StartFireAction(FInputActionValue());
			ac->GetFSM()->SenseNewActor(target);
		}
		else
		{
			ac->MoveToActor(target, attackDist - 100.0f, true, true, true);
		}

		targetLastLoc = target->GetActorLocation();
	}
	// 보이지 않는다면 마지막으로 발견한 타겟의 위치로 이동, 일정 시간 동안 안 보이면 patrol 상태로 변경
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Missing..."))

		WeaponComp->EndFireAction(FInputActionValue());
		SenseNewActor(nullptr);
		ac->MoveToLocation(targetLastLoc, 0.f, true, true);
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
		GetWorld()->GetTimerManager().ClearTimer(handle);
		GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda([&]
		{
			target = nullptr;
			ac->SetContext(EEnemystate::patrol);
		}), missingTime, false, missingTime);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(handle);
		target = NewActor;
	}
}

