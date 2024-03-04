// copyright by LootingEmployment


#include "AIComponent/FSM_Patrol_Component.h"

#include "AIController/EOSAIController.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

UFSM_Patrol_Component::UFSM_Patrol_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Patrol_Component::BeginPlay()
{
	Super::BeginPlay();
}

bool UFSM_Patrol_Component::IsAtDestination()
{
	if (NextWaypoint == nullptr) return false;
	
	float dist = FVector::Dist(NextWaypoint->GetActorLocation(), ai->GetActorLocation());
	return dist <= 100;
}

void UFSM_Patrol_Component::MultiRPCPerformLookAround_Implementation(UAnimMontage* MontageToPlay)
{
	UE_LOG(LogTemp, Warning, TEXT("play montage"));
	if (MontageToPlay && ai->GetMesh()->GetAnimInstance())
	{
		ai->GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
	}
}

void UFSM_Patrol_Component::SetNextDestination()
{
	if (!bHasNextWaypoint)
	{
		bHasNextWaypoint = true;
		NextWaypoint = waypointArray[CurrentWaypointIndex];
		CurrentWaypointIndex = (CurrentWaypointIndex + 1) % waypointArray.Num();
	}

	GetWorld()->GetTimerManager().ClearTimer(LookAroundTimerhandle);
}

void UFSM_Patrol_Component::MoveToDestination()
{
	ac->MoveToActor(NextWaypoint, AcceptanceRadius, true, true, false, 0, true);
}

void UFSM_Patrol_Component::ExecuteBehavior()
{
	if (waypointArray.Num() == 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("waypoint list is empty"));
		return;
	}

	if (!bMumble)
	{
		bMumble = true;
		
		int32 randValue = FMath::RandRange(0,9);
		if (randValue == 0)
		{
			mumbleSound = mumble1;
		}
		else if (randValue == 1)
		{
			mumbleSound = mumble2;
		}
		else if (randValue == 2)
		{
			mumbleSound = mumble3;
		}
		else
		{
			mumbleSound = nullptr;
		}

		if (mumbleSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), mumbleSound, ai->GetActorLocation(), ai->GetActorRotation());
		}
		float duration = (mumbleSound != nullptr ? (mumbleSound->Duration + mumblingTime) : mumblingTime);
		
		GetWorld()->GetTimerManager().SetTimer(mumbleTimerhandle, this, &UFSM_Patrol_Component::OnMumbleTimerExpired, duration, false);
	}
	
	NextWaypoint = waypointArray[CurrentWaypointIndex];
	
	if (IsAtDestination())
	{
		if (!bHasPerformedLookAround)
		{
			MultiRPCPerformLookAround(lookAroundMontage);
			bHasPerformedLookAround = true;
		}
		else
		{
			if (LookAroundTimerhandle.IsValid()) return;

			GetWorld()->GetTimerManager().SetTimer(LookAroundTimerhandle, this, &UFSM_Patrol_Component::OnLookAtroundTimerExpired, lookAroundMontage->GetPlayLength(), false);
		}
	}
	else
	{
		bHasNextWaypoint = false;
		MoveToDestination();
	}
}

void UFSM_Patrol_Component::StopExecute()
{
	ac->StopMovement();
	GetWorld()->GetTimerManager().ClearTimer(LookAroundTimerhandle);
}

void UFSM_Patrol_Component::SenseNewActor(AActor* NewActor)
{
	if (NewActor == nullptr)
	{
		return;
	}
	
	ac->SetContext(EEnemystate::search);
	ac->GetFSM()->SenseNewActor(NewActor);
}

void UFSM_Patrol_Component::OnLookAtroundTimerExpired()
{
	bHasPerformedLookAround = false;
	SetNextDestination();
}

void UFSM_Patrol_Component::OnMumbleTimerExpired()
{
	bMumble = false;
}

