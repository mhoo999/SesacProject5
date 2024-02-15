// copyright by LootingEmployment


#include "Component/FSM_Attack_Component.h"

#include "AIController/HoonsAIController.h"
#include "Character/CharacterBase.h"
#include "Component/WeaponComponent.h"

UFSM_Attack_Component::UFSM_Attack_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Attack_Component::BeginPlay()
{
	Super::BeginPlay();

	ac = Cast<AHoonsAIController>(GetOwner());
	ai = Cast<ACharacterBase>(ac->GetPawn());

	WeaponComp = ai->GetComponentByClass<UWeaponComponent>();
}

void UFSM_Attack_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFSM_Attack_Component::ExecuteBehavior()
{
	FHitResult hitResult;
	FVector start = ai->GetActorLocation();
	FVector end = target->GetActorLocation();
	FCollisionQueryParams params;

	GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility);

	if (hitResult.GetActor() != nullptr && hitResult.GetActor() == ai)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire!"));
	}
	else
	{
		ac->SetContext(EEnemystate::chase);
	}
	
	float dist = FVector::Dist(target->GetActorLocation(), ai->GetActorLocation());
	float attackDist = WeaponComp->GetWeaponAttackRange();

	if (dist > attackDist)
	{
		ac->SetContext(EEnemystate::chase);
	}
}

void UFSM_Attack_Component::StopExecute()
{
	ac->StopMovement();
}

void UFSM_Attack_Component::SenseNewActor(AActor* NewActor)
{
	target = NewActor;
	
	if (NewActor == nullptr)
	{
		target = nullptr;
		ac->SetContext(EEnemystate::chase);
	}
}

