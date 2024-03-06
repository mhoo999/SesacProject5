// copyright by LootingEmployment


#include "Component/EscapeComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEscapeComponent::UEscapeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEscapeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetOwner()->HasAuthority())
	{
		SetIsReplicated(true);
	}
	EscapeTime = MaxEscapeTime;
}

// Called every frame
void UEscapeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (false == bIsEscaping) return;
	
	EscapeTime -= DeltaTime;
	if (EscapeTime <= 0.f)
	{
		EscapeTime = 0.f;

		if (GetOwner()->HasAuthority())
		{
			OnEscape.Broadcast();
		}
	}
	OnRep_EscapeTime();
}

void UEscapeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UEscapeComponent, bIsEscaping, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UEscapeComponent, EscapeTime, COND_OwnerOnly);
}

void UEscapeComponent::StartEscape(AExitBase* CurrentExit)
{
	if (ExitArray.Find(CurrentExit) == INDEX_NONE) return;
	
	if (true == bIsEscaping) return;
	bIsEscaping = true;
	OnRep_IsEscaping();
	UE_LOG(LogTemp, Warning, TEXT("UEscapeComponent::StartEscape"));
}

void UEscapeComponent::EndEscape(AExitBase* CurrentExit)
{
	if (ExitArray.Find(CurrentExit) == INDEX_NONE) return;
	
	if (false == bIsEscaping) return;
	bIsEscaping = false;
	OnRep_IsEscaping();
	UE_LOG(LogTemp, Warning, TEXT("UEscapeComponent::EndEscape"));
	EscapeTime = MaxEscapeTime;
}

void UEscapeComponent::SetExitArray(TArray<AExitBase*> NewExitArray)
{
	ExitArray = NewExitArray;
}

const TArray<AExitBase*>& UEscapeComponent::GetExitArray() const
{
	return ExitArray;
}

void UEscapeComponent::ClientRPC_Escape_Implementation()
{
	// UE_LOG(LogTemp, Warning, TEXT("UEscapeComponent::ClientRPC_Escape_Implementation"));

	if (GetOwner()->HasAuthority() == false)
	{
		OnEscape.Broadcast();	
	}
	GetWorld()->GetFirstPlayerController()->ClientTravel("/Game/YMH/Level/Result_YMH", TRAVEL_Absolute);
}

void UEscapeComponent::OnRep_IsEscaping()
{
	// UE_LOG(LogTemp, Warning, TEXT("UEscapeComponent::OnRep_IsEscaping"));
	OnIsEscapingChanged.ExecuteIfBound(bIsEscaping);
}

void UEscapeComponent::OnRep_EscapeTime()
{
	OnEscapeTimeChanged.ExecuteIfBound(EscapeTime);
}