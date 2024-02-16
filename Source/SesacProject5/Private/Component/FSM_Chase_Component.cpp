// copyright by LootingEmployment


#include "Component/FSM_Chase_Component.h"

#include "InputActionValue.h"
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

	// 추후 플레이어 Health 파트를 부위별로 돌면서 사격 가능한 부분을 PlayerPartLocation에 저장하고 사격 개시
	// 파트별로 우선순위를 정해 보이는 부분 중 가장 점수가 높은 파트를 1순위로 저장
	// for (auto part : )

	if (target)
	{
		PlayerPartLocation = target->GetActorLocation();
		bCanSeeTarget = GetWorld()->LineTraceSingleByChannel(hitResult, AIEyeLocation, PlayerPartLocation, ECC_Visibility);
		DrawDebugLine(GetWorld(), AIEyeLocation, PlayerPartLocation, FColor::Magenta, false, 0.5f, 0, 2.0f);
		// UE_LOG(LogTemp, Warning, TEXT("%p"), hitResult.GetActor());
	}
	
	// 시야에 타겟이 보인다면 공격
	if (bCanSeeTarget && hitResult.GetActor() == target)
	{
		float dist = FVector::Dist(target->GetActorLocation(), ai->GetActorLocation());
		float attackDist = WeaponComp->GetWeaponAttackRange();

		UE_LOG(LogTemp, Warning, TEXT("Dist : %f, AttackDist : %f"), dist, attackDist);
		
		if (dist <= attackDist)
		{
			ac->StopMovement();
			WeaponComp->StartFireAction(FInputActionValue());
			UE_LOG(LogTemp, Warning, TEXT("Fire!"));
			ac->GetFSM()->SenseNewActor(target);
		}
		else
		{
			ac->MoveToActor(target, attackDist - 100.0f, true, true, true);
		}

		dest = target->GetActorLocation();
	}
	// 보이지 않는다면 마지막으로 발견한 타겟의 위치로 이동, 일정 시간 동안 안 보이면 patrol 상태로 변경
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Missing..."))

		WeaponComp->EndFireAction(FInputActionValue());
		SenseNewActor(nullptr);
		ac->MoveToLocation(dest, 0.f, true, true);
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

