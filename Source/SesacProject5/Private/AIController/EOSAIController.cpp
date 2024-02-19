// copyright by LootingEmployment


#include "AIController/EOSAIController.h"

#include "Character/CharacterBase.h"
#include "Component/FSM_Chase_Component.h"
#include "Component/FSM_Patrol_Component.h"
#include "Component/FSM_Search_Component.h"
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

	sight->SightRadius = 1500.f;
	sight->LoseSightRadius = sight->SightRadius + 500.f;
	sight->PeripheralVisionAngleDegrees = 65.f;
	sight->DetectionByAffiliation.bDetectNeutrals = true;
	sight->DetectionByAffiliation.bDetectFriendlies = true;
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
	printLog();
}

void AEOSAIController::ChangeDead(bool bNewIsDead)
{
	if (bNewIsDead)
	{
		FSMInterface->StopExecute();
		SetActorTickEnabled(false);
	}
}

void AEOSAIController::OnPerception(AActor* actor, FAIStimulus stimulus)
{
	ACharacterBase* chr = Cast<ACharacterBase>(actor);
	if (chr == nullptr)
	{
		return;
	}

	// SetFocus 센싱 성공 ? chr 반환 : nullptr 반환
	// 0219 (&& ai->TeamId != chr->TeamId && chr->TeamId != 255) 추가하여 적일 경우에만 chr 반환하도록 수정 
	SetFocus(stimulus.WasSuccessfullySensed() ? chr : nullptr);
	
	// UE_LOG(LogTemp, Warning, TEXT("%ls"), (chr->TeamId == 1) ? TEXT("Friend") : TEXT("Enemy"));

	if (chr)
	{
		FSMInterface->SenseNewActor(chr);
	}
}

void AEOSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("AEOSAIController::OnPossess) Pawn : %s"), *InPawn->GetActorNameOrLabel());
	// register the onPerceptionUpdated function to fire whenever the AIPerception get's updated
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEOSAIController::OnPerception);

	ai = Cast<ACharacterBase>(GetPawn());
	TArray<UActorComponent*> ComponentArray = this->K2_GetComponentsByClass(UFSM_Component::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("ComponentArray : %d"), ComponentArray.Num());
	for (auto Iter : ComponentArray)
	{
		Cast<UFSM_Component>(Iter)->Init(InPawn);
	}
	// InitDelegate.Broadcast();
	ai->GetComponentByClass<UHealthComponent>()->OnIsDeadChanged.AddUObject(this, &AEOSAIController::ChangeDead);
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

