// copyright by LootingEmployment


#include "AIController/EOSAIController.h"

#include "Character/CharacterBase.h"
#include "AIComponent/FSM_Chase_Component.h"
#include "AIComponent/FSM_Patrol_Component.h"
#include "AIComponent/FSM_Search_Component.h"
#include "Component/HealthComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AEOSAIController::AEOSAIController()
{
	FSMPatrolComp = CreateDefaultSubobject<UFSM_Patrol_Component>(TEXT("Patrol Component"));
	FSMSearchComp = CreateDefaultSubobject<UFSM_Search_Component>(TEXT("Search Component"));
	FSMChaseComp = CreateDefaultSubobject<UFSM_Chase_Component>(TEXT("Chase Component"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	
	// create a sight and hearing sense
	sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	// register the sight sense to our perception component
	AIPerception->ConfigureSense(*sight);
	AIPerception->ConfigureSense(*hearing);
	AIPerception->SetDominantSense(sight->GetSenseImplementation());
	AIPerception->SetDominantSense(hearing->GetSenseImplementation());

	sight->SightRadius = 3000.f;
	sight->LoseSightRadius = sight->SightRadius + 250.f;
	sight->PeripheralVisionAngleDegrees = 65.f;
	sight->DetectionByAffiliation.bDetectNeutrals = true;
	sight->DetectionByAffiliation.bDetectFriendlies = true;

	hearing->HearingRange = 500.f;
	hearing->DetectionByAffiliation.bDetectNeutrals = true;
	hearing->DetectionByAffiliation.bDetectFriendlies = true;

	bReplicates = true;
}

void AEOSAIController::BeginPlay()
{
	Super::BeginPlay();

	FSMInterface = FSMPatrolComp;
	state = EEnemystate::patrol;
}

void AEOSAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FSMInterface->ExecuteBehavior();
	// printLog();
}

void AEOSAIController::ChangeDead(bool bNewIsDead)
{
	if (bNewIsDead)
	{
		FSMInterface->StopExecute();
		this->Destroy();
	}
}

void AEOSAIController::OnPerception(AActor* actor, FAIStimulus stimulus)
{
	ACharacterBase* chr = Cast<ACharacterBase>(actor);
	if (chr == nullptr)
	{
		return;
	}
	
	// SetFocus(stimulus.WasSuccessfullySensed() ? chr  : nullptr);

	if (stimulus.WasSuccessfullySensed() && chr->TeamId != ai->TeamId)
	{
		SetContext(EEnemystate::search);
		FSMInterface->SenseNewActor(actor);
	}
}

void AEOSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// UE_LOG(LogTemp, Warning, TEXT("AEOSAIController::OnPossess) Pawn : %s"), *InPawn->GetActorNameOrLabel());
	// register the onPerceptionUpdated function to fire whenever the AIPerception get's updated
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEOSAIController::OnPerception);

	ai = Cast<ACharacterBase>(GetPawn());
	TArray<UActorComponent*> ComponentArray = this->K2_GetComponentsByClass(UFSM_Component::StaticClass());
	// UE_LOG(LogTemp, Warning, TEXT("ComponentArray : %d"), ComponentArray.Num());
	for (auto Iter : ComponentArray)
	{
		Cast<UFSM_Component>(Iter)->Init(InPawn);
	}
	// InitDelegate.Broadcast();
	ai->GetComponentByClass<UHealthComponent>()->OnIsDeadChanged.AddUObject(this, &AEOSAIController::ChangeDead);
	ai->GetComponentByClass<UHealthComponent>()->OnAttacked.AddUObject(this, &AEOSAIController::beAttacked);
}

void AEOSAIController::SetContext(EEnemystate next)
{
	FSMInterface->StopExecute();
	
	switch (next) {
	case EEnemystate::patrol:			FSMInterface = FSMPatrolComp;			break;
	case EEnemystate::search:			FSMInterface = FSMSearchComp;			break;
	case EEnemystate::chase:			FSMInterface = FSMChaseComp;			break;
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
	state = next;
}

IFSMInterface* AEOSAIController::GetFSM()
{
	return FSMInterface;
}

void AEOSAIController::printLog()
{
	FString StateString = UEnum::GetValueAsString(state);
	FString CleanStateString = StateString.Mid(StateString.Find(TEXT("."), ESearchCase::IgnoreCase, ESearchDir::FromEnd) + 1);

	if (ai)
	{
		DrawDebugString(GetWorld(), ai->GetActorLocation(), CleanStateString, nullptr, FColor::Yellow, 0, true, 1);
	}
}

void AEOSAIController::SetWaypoint(TArray<AActor*> waypointArray)
{
	FSMPatrolComp->waypointArray = waypointArray;
}

void AEOSAIController::beAttacked(AActor* attacker)
{
	SetContext(EEnemystate::chase);
	FSMInterface->SenseNewActor(attacker);
}

