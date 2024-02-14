// copyright by LootingEmployment


#include "AIController/HoonsAIController.h"

#include "Character/CharacterBase.h"
#include "Component/FSM_Patrol_Component.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AHoonsAIController::AHoonsAIController()
{
	FSMPatrolComp = CreateDefaultSubobject<UFSM_Patrol_Component>(TEXT("Patrol Component"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));

	// create a sight and hearing sense
	sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	sight->SightRadius = 2000.f;
	sight->LoseSightRadius = sight->SightRadius + 500.f;
	sight->PeripheralVisionAngleDegrees = 90.f;
	sight->DetectionByAffiliation.bDetectNeutrals = true;

	// register the sight sense to our perception component
	AIPerception->ConfigureSense(*sight);
	AIPerception->SetDominantSense(sight->GetSenseImplementation());
}

void AHoonsAIController::BeginPlay()
{
	Super::BeginPlay();

	FSMInterface = Cast<IFSMInterface>(FSMPatrolComp);
	state = EEnemystate::patrol;

	ACharacterBase* chr = Cast<ACharacterBase>(GetPawn());
	if (chr)
	{
		Agent = chr;
	}
}

void AHoonsAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	switch (state)
	{
	case EEnemystate::patrol:			FSMInterface->ExecutePatrol();			break;
	case EEnemystate::search:			FSMInterface->ExecuteSearch();			break;
	case EEnemystate::chase:			FSMInterface->ExecuteChase();			break;
	case EEnemystate::attack:			FSMInterface->ExecuteAttack();			break;
	case EEnemystate::retreatFiring:	FSMInterface->ExecuteRetreatFiring();	break;
	case EEnemystate::AdvanceFiring:	FSMInterface->ExecuteAdvanceFiring();	break;
	case EEnemystate::evade:			FSMInterface->ExecuteEvade();			break;
	case EEnemystate::camping:			FSMInterface->ExecuteCamping();			break;
	case EEnemystate::selfHealing:		FSMInterface->ExecuteSelfHealing();		break;
	}
}

void AHoonsAIController::OnPerception(AActor* actor, FAIStimulus stimulus)
{
	ACharacterBase* chr = Cast<ACharacterBase>(actor);
	if (chr == nullptr)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("%ls"), (chr->TeamId == 1) ? TEXT("Friend") : TEXT("Enemy"));
	SetFocus(stimulus.WasSuccessfullySensed() ? chr : nullptr);
}

void AHoonsAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// register the onPerceptionUpdated function to fire whenever the AIPerception get's updated
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AHoonsAIController::OnPerception);
}

