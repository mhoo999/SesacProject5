// copyright by LootingEmployment


#include "Component/FSM_Chase_Component.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/SPIRV-Headers/include/spirv/unified1/spirv.h>

#include "InputActionValue.h"
#include "AIController/EOSAIController.h"
#include "Character/CharacterBase.h"
#include "Component/AIWeaponComponent.h"
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
	FVector TargetLocation;
	if (FocusTargetPart(target, TargetLocation))
	{
		float dist = FVector::Dist(target->GetActorLocation(), ai->GetActorLocation());
		float attackDist = WeaponComp->GetWeaponAttackRange();

		DrawDebugLine(GetWorld(), ai->GetActorLocation(), TargetLocation, FColor::Red);

		// UE_LOG(LogTemp, Warning, TEXT("Dist : %f, attackDist : %f"), dist, attackDist);
		
		if (dist <= attackDist)
		{
			// ac->SetFocalPoint(target->GetActorLocation() + FVector(500, 0, 0));
			if (bIsAttacking == false)
			{
				bIsAttacking = true;
				ac->StopMovement();
				WeaponComp->SetFocusLocation(TargetLocation);
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

bool UFSM_Chase_Component::FocusTargetPart(AActor* targetActor, FVector& TargetLocation)
{
	if (targetActor == nullptr) return false;
	
	// FVector targetLoc;

	ACharacter* TargetCharacter = Cast<ACharacter>(targetActor);

	if (TargetCharacter == nullptr) return false;

	
	// USkeletalMeshComponent* SkeletalMeshComponent = targetActor->FindComponentByClass<USkeletalMeshComponent>();
	// const TArray<USkeletalMeshSocket*>& Sockets = SkeletalMeshComponent->SkeletalMesh->GetMeshOnlySocketList();
	// const TArray<USkeletalMeshSocket*>& Sockets = SkeletalMeshComponent->GetSkeletalMeshAsset()->GetMeshOnlySocketList();
	TArray<FName> Sockets = TargetCharacter->GetMesh()->GetAllSocketNames();

	UE_LOG(LogTemp, Warning, TEXT("%d"), Sockets.Num());
	// UE_LOG(LogTemp, Warning, TEXT("%s"), fstr Sockets);

	TargetPart currentTarget = {"NONE", 4, true};
	
	for (FName Socket : Sockets)
	{
		// UE_LOG(LogTemp, Warning, TEXT("in Socket Iter %s"), *Socket.ToString());
		
		FString SocketName = Socket.ToString();
		FVector SocketLocation = TargetCharacter->GetMesh()->GetSocketLocation(Socket);

		FHitResult checkResult;
		if (GetWorld()->LineTraceSingleByChannel(checkResult, ai->GetActorLocation(), SocketLocation, ECC_Visibility))
		{
			if (SocketName.Equals("spine_03") && checkResult.GetActor() == targetActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("in Socket Iter == spine_03"));
				TargetPart THORAX {"THORAX", 1, true, SocketLocation};
				if (THORAX.priority < currentTarget.priority)
				{
					currentTarget = THORAX;
				}
			}
			else if (SocketName.Equals("spine_01") && checkResult.GetActor() == targetActor)
			{
				TargetPart STOMACH {"STOMACH", 2, true, SocketLocation};
				if (STOMACH.priority < currentTarget.priority)
				{
					currentTarget = STOMACH;
				}
			}
			else if (SocketName.Equals("head") && checkResult.GetActor() == targetActor)
			{
				TargetPart HEAD {"HEAD", 3, true, SocketLocation};
				if (HEAD.priority < currentTarget.priority)
				{
					currentTarget = HEAD;
				}
			}
			else if (SocketName.Equals("Hand_R") && checkResult.GetActor() == targetActor)
			{
				TargetPart RIGHTARM {"RIGHTARM", 3, true, SocketLocation};
				if (RIGHTARM.priority < currentTarget.priority)
				{
					currentTarget = RIGHTARM;
				}
			}
			else if (SocketName.Equals("Hand_L") && checkResult.GetActor() == targetActor)
			{
				TargetPart LEFTARM {"LEFTARM", 3, true, SocketLocation};
				if (LEFTARM.priority < currentTarget.priority)
				{
					currentTarget = LEFTARM;
				}
			}
			else if (SocketName.Equals("calf_r") && checkResult.GetActor() == targetActor)
			{
				TargetPart RIGHTLEG {"RIGHTLEG", 3, true, SocketLocation};
				if (RIGHTLEG.priority < currentTarget.priority)
				{
					currentTarget = RIGHTLEG;
				}
			}
			else if (SocketName.Equals("calf_l") && checkResult.GetActor() == targetActor)
			{
				TargetPart LEFTLEG {"LEFTLEG", 3, true, SocketLocation};
				if (LEFTLEG.priority < currentTarget.priority)
				{
					currentTarget = LEFTLEG;
				}
			}
		}	
		// UE_LOG(LogTemp, Warning, TEXT("current Tatget : %s"), *FString(currentTarget.name.c_str()));
	}

	if (currentTarget.name == "NONE")
	{
		return false;
	}
	
	TargetLocation = currentTarget.partLoc;
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(targetLoc.ToString()));
	return true;
}

