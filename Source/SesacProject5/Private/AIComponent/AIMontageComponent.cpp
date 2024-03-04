// copyright by LootingEmployment


#include "AIComponent/AIMontageComponent.h"

#include "Character/ScavBase.h"

// Sets default values for this component's properties
UAIMontageComponent::UAIMontageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAIMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	SetIsReplicated(true);
}


// Called every frame
void UAIMontageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UAIMontageComponent::getLookAtoundMontageTime()
{
	return lookAroundMontage->GetPlayLength();
}

void UAIMontageComponent::MultiRPCPlayLookAround_Implementation()
{
	auto ai = Cast<AScavBase>(GetOwner());
	ai->GetMesh()->GetAnimInstance()->Montage_Play(lookAroundMontage);

	UE_LOG(LogTemp,	Warning, TEXT("PlayLookAround"));
}

