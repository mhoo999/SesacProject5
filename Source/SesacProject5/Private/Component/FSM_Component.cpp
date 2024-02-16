// copyright by LootingEmployment


#include "Component/FSM_Component.h"

#include "AIController/EOSAIController.h"
#include "Character/CharacterBase.h"
#include "Component/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"

UFSM_Component::UFSM_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Component::BeginPlay()
{
	Super::BeginPlay();


	ac = Cast<AEOSAIController>(GetOwner());
	ai = Cast<ACharacterBase>(ac->GetPawn());
	WeaponComp = ai->GetComponentByClass<UWeaponComponent>();
}

bool UFSM_Component::bFocusTarget() const
{
	// 추후 플레이어 Health 파트를 부위별로 돌면서 사격 가능한 부분을 PlayerPartLocation에 저장하고 사격 개시
	// 파트별로 우선순위를 정해 보이는 부분 중 가장 점수가 높은 파트를 1순위로 저장
	// for (auto part : )
	
	// FHitResult hitResult;
	// FVector AIEyeLocation = ai->GetMesh()->GetSocketLocation("eyes");
	FVector AILocation = ai->GetActorLocation();
	FVector PlayerPartLocation = target->GetActorLocation();
	// bool bCanSeeTarget = GetWorld()->LineTraceSingleByChannel(hitResult, AIEyeLocation, PlayerPartLocation, ECC_Visibility);
	TArray<FHitResult> OutHits;
	bool bTarget = false;
	if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), AILocation, PlayerPartLocation,
			sightRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, {},
			EDrawDebugTrace::ForOneFrame, OutHits, true))
	{
		for (auto it : OutHits)
		{
			if (it.GetActor() == target)
			{
				bTarget = true;
			}
		}
	}
	
	// DrawDebugLine(GetWorld(), AIEyeLocation, PlayerPartLocation, FColor::Magenta, false, 0.1f, 0, 10.0f);
	
	return bTarget;
}

