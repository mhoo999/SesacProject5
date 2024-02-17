// copyright by LootingEmployment


#include "Character/ScavBase.h"

#include "AIController/EOSAIController.h"

AScavBase::AScavBase()
{
	static ConstructorHelpers::FClassFinder<AEOSAIController> EOSAIController(TEXT("/Game/YMH/Blueprint/BP_AIController_YMH.BP_AIController_YMH_C"));
	if (EOSAIController.Succeeded())
	{
		AIControllerClass = EOSAIController.Class;
	}

	TeamId = FGenericTeamId(1);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AScavBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetMesh()->GetSocketLocation("eyes");
	OutRotation = GetMesh()->GetSocketRotation("eyes") + FRotator(0, 90, 0);
}
