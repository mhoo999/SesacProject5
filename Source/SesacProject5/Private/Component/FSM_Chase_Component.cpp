// copyright by LootingEmployment


#include "Component/FSM_Chase_Component.h"

#include "AIController/HoonsAIController.h"
#include "Character/CharacterBase.h"

UFSM_Chase_Component::UFSM_Chase_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Chase_Component::BeginPlay()
{
	Super::BeginPlay();

	ac = Cast<AHoonsAIController>(GetOwner());
	ai = Cast<ACharacterBase>(ac->GetPawn());

	attackDist = ac->AttackDist;
}

void UFSM_Chase_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFSM_Chase_Component::ExecuteBehavior(AActor* target)
{
	UE_LOG(LogTemp, Warning, TEXT("Chase!"));

	float dist = FVector::Dist(target->GetActorLocation(), ai->GetActorLocation());

	ac->MoveToActor(target, 10.f, true, true, false, 0, true);
	
	if (dist <= attackDist)
	{
		// ac->StopMovement();
		ac->SetContext(EEnemystate::attack);
	}
}

void UFSM_Chase_Component::StopExecute()
{
}

void UFSM_Chase_Component::SenseNewActor(AActor* NewActor)
{
}

