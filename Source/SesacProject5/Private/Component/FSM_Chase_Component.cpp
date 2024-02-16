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
	FVector AIEyeLocation = ai->GetMesh()->GetSocketLocation("eyes");
	FVector PlayerPartLocation = target->GetActorLocation();

	// 추후 플레이어 Health 파트를 부위별로 돌면서 사격 가능한 부분을 PlayerPartLocation에 저장하고 사격 개시
	// 파트별로 우선순위를 정해 보이는 부분 중 가장 점수가 높은 파트를 1순위로 저장
	// for (auto part : )

	FHitResult hitResult;
	GetWorld()->LineTraceSingleByChannel(hitResult, AIEyeLocation, PlayerPartLocation, ECC_Visibility);
	DrawDebugLine(GetWorld(), AIEyeLocation, PlayerPartLocation, FColor::Magenta, false, 0.5f, 0, 2.0f);

	UE_LOG(LogTemp, Warning, TEXT("%p"), hitResult.GetActor());
	
	// 시야에 보인다면 공격 거리 까지 이동
	if (hitResult.GetActor() == target)
	{
		float dist = FVector::Dist(target->GetActorLocation(), ai->GetActorLocation());
		float attackDist = WeaponComp->GetWeaponAttackRange();

		// 공격 거리 내라면 공격
		if (dist <= attackDist)
		{
			ac->StopMovement();
			UE_LOG(LogTemp, Warning, TEXT("Fire!"));
			// WeaponComp->StartFireAction();
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

