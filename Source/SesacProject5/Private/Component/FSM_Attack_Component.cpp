// copyright by LootingEmployment


#include "Component/FSM_Attack_Component.h"

#include "AIController/HoonsAIController.h"
#include "Character/CharacterBase.h"

UFSM_Attack_Component::UFSM_Attack_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Attack_Component::BeginPlay()
{
	Super::BeginPlay();

	ac = Cast<AHoonsAIController>(GetOwner());
	ai = Cast<ACharacterBase>(ac->GetPawn());

	attackDist = ac->AttackDist;
}

void UFSM_Attack_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFSM_Attack_Component::ExecuteBehavior(AActor* target)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire!"));

	float dist = FVector::Dist(target->GetActorLocation(), ai->GetActorLocation());

	if (dist > attackDist)
	{
		ac->SetContext(EEnemystate::chase);
	}
}

void UFSM_Attack_Component::StopExecute()
{
}

void UFSM_Attack_Component::SenseNewActor(AActor* NewActor)
{
}

