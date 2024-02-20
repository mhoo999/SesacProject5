// copyright by LootingEmployment


#include "Component/FSM_Chase_Component.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/SPIRV-Headers/include/spirv/unified1/spirv.h>

#include "InputActionValue.h"
#include "AIController/EOSAIController.h"
#include "Character/CharacterBase.h"
#include "Component/WeaponComponent.h"
#include "Engine/SkeletalMeshSocket.h"

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

		// UE_LOG(LogTemp, Warning, TEXT("Dist : %f, attackDist : %f"), dist, attackDist);
		
		if (dist <= attackDist)
		{
			// ac->SetFocalPoint(target->GetActorLocation() + FVector(500, 0, 0));
			if (bIsAttacking == false)
			{
				FocusTargetPart(target);
				
				bIsAttacking = true;
				ac->StopMovement();
				WeaponComp->StartFireAction(FInputActionValue());
				// ac->GetFSM()->SenseNewActor(target);
			}
			else
			{
				ai->FaceRotation(ai->GetControlRotation() + FRotator(10, 0, 0));
			}
		}
		else
		{
			bIsAttacking = false;
			WeaponComp->EndFireAction(FInputActionValue());
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

FVector UFSM_Chase_Component::FocusTargetPart(AActor* targetActor)
{
	if (targetActor == nullptr) return FVector(NULL);
	
	FVector targetLoc;
	
	USkeletalMeshComponent* SkeletalMeshComponent = targetActor->FindComponentByClass<USkeletalMeshComponent>();
	// const TArray<USkeletalMeshSocket*>& Sockets = SkeletalMeshComponent->SkeletalMesh->GetMeshOnlySocketList();
	const TArray<USkeletalMeshSocket*>& Sockets = SkeletalMeshComponent->GetSkeletalMeshAsset()->GetMeshOnlySocketList();

	for (USkeletalMeshSocket* Socket : Sockets)
	{
		FString SocketName = Socket->SocketName.ToString();
		FVector SocketLocation = Socket->GetSocketLocation(SkeletalMeshComponent);
		TargetPart currentTarget = {"없음", 4, true};

		FHitResult checkResult;
		GetWorld()->LineTraceSingleByChannel(checkResult, ai->GetActorLocation(), SocketLocation, ECC_Visibility);
		
		if (SocketName == "spine_03" && checkResult.GetActor() == target)
		{
			TargetPart THORAX {"가슴", 1, true, SocketLocation};
			if (THORAX.priority > currentTarget.priority)
			{
				currentTarget = THORAX;
			}
		}
		else if (SocketName == "spine_01" && checkResult.GetActor() == target)
		{
			TargetPart STOMACH {"복부", 2, true, SocketLocation};
			if (STOMACH.priority > currentTarget.priority)
			{
				currentTarget = STOMACH;
			}
		}
		else if (SocketName == "head" && checkResult.GetActor() == target)
		{
			TargetPart HEAD {"머리", 3, true, SocketLocation};
			if (HEAD.priority > currentTarget.priority)
			{
				currentTarget = HEAD;
			}
		}
		else if (SocketName == "Hand_R" && checkResult.GetActor() == target)
		{
			TargetPart RIGHTARM {"오른팔", 3, true, SocketLocation};
			if (RIGHTARM.priority > currentTarget.priority)
			{
				currentTarget = RIGHTARM;
			}
		}
		else if (SocketName == "Hand_L" && checkResult.GetActor() == target)
		{
			TargetPart LEFTARM {"왼팔", 3, true, SocketLocation};
			if (LEFTARM.priority > currentTarget.priority)
			{
				currentTarget = LEFTARM;
			}
		}
		else if (SocketName == "calf_r" && checkResult.GetActor() == target)
		{
			TargetPart RIGHTLEG {"오른발", 3, true, SocketLocation};
			if (RIGHTLEG.priority > currentTarget.priority)
			{
				currentTarget = RIGHTLEG;
			}
		}
		else if (SocketName == "calf_l" && checkResult.GetActor() == target)
		{
			TargetPart LEFTLEG {"왼발", 3, true, SocketLocation};
			if (LEFTLEG.priority > currentTarget.priority)
			{
				currentTarget = LEFTLEG;
			}
		}
		else
		{
			currentTarget = {"없음", 4, true};
		}
		
		UE_LOG(LogTemp, Warning, TEXT("current Tatget : %s"), *FString(currentTarget.name.c_str()));
		targetLoc = currentTarget.partLoc;
	}

	return targetLoc;
}

