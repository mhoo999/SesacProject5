// copyright by LootingEmployment


#include "AIController/HoonsAIController.h"

#include "Character/CharacterBase.h"
#include "Component/FSM_Attack_Component.h"
#include "Component/FSM_Chase_Component.h"
#include "Component/FSM_Patrol_Component.h"
#include "Component/FSM_Search_Component.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AHoonsAIController::AHoonsAIController()
{
	FSMPatrolComp = CreateDefaultSubobject<UFSM_Patrol_Component>(TEXT("Patrol Component"));
	FSMSearchComp = CreateDefaultSubobject<UFSM_Search_Component>(TEXT("Search Component"));
	FSMChaseComp = CreateDefaultSubobject<UFSM_Chase_Component>(TEXT("Chase Component"));
	FSMAttackComp = CreateDefaultSubobject<UFSM_Attack_Component>(TEXT("Attack Component"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	
	// create a sight and hearing sense
	sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	// register the sight sense to our perception component
	AIPerception->ConfigureSense(*sight);
	AIPerception->ConfigureSense(*hearing);
	AIPerception->SetDominantSense(sight->GetSenseImplementation());
	AIPerception->SetDominantSense(hearing->GetSenseImplementation());

	sight->SightRadius = 1000.f;
	sight->LoseSightRadius = sight->SightRadius + 500.f;
	sight->PeripheralVisionAngleDegrees = 90.f;
	sight->DetectionByAffiliation.bDetectNeutrals = true;
	sight->DetectionByAffiliation.bDetectFriendlies = true;
}

void AHoonsAIController::BeginPlay()
{
	Super::BeginPlay();

	FSMInterface = FSMPatrolComp;
	state = EEnemystate::patrol;

	ai = Cast<ACharacterBase>(GetPawn());
}

void AHoonsAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FSMInterface->ExecuteBehavior(target);
}

void AHoonsAIController::OnPerception(AActor* actor, FAIStimulus stimulus)
{
	ACharacterBase* chr = Cast<ACharacterBase>(actor);
	if (chr == nullptr)
	{
		return;
	}
	SetFocus(stimulus.WasSuccessfullySensed() ? chr : nullptr);
	
	UE_LOG(LogTemp, Warning, TEXT("%ls"), (chr->TeamId == 1) ? TEXT("Friend") : TEXT("Enemy"));

	// 캐릭터, 아이템 등 인식 가능하도록 개선 필요
	if (ai->TeamId != chr->TeamId && chr->TeamId != 255)
	{
		target = chr;
		FSMInterface->SenseNewActor(chr);
	}
}

void AHoonsAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// register the onPerceptionUpdated function to fire whenever the AIPerception get's updated
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AHoonsAIController::OnPerception);
}

void AHoonsAIController::SetContext(EEnemystate next)
{
	FSMInterface->StopExecute();
	
	switch (next) {
	case EEnemystate::patrol:			FSMInterface = FSMPatrolComp;			break;
	case EEnemystate::search:			FSMInterface = FSMSearchComp;			break;
	case EEnemystate::chase:			FSMInterface = FSMChaseComp;			break;
	case EEnemystate::attack:			FSMInterface = FSMAttackComp;			break;
	case EEnemystate::retreatFiring:
		break;
	case EEnemystate::AdvanceFiring:
		break;
	case EEnemystate::evade:
		break;
	case EEnemystate::camping:
		break;
	case EEnemystate::selfHealing:
		break;
	}
}

