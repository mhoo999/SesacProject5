// copyright by LootingEmployment


#include "Object/Openable.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AOpenable::AOpenable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AOpenable::BeginPlay()
{
	Super::BeginPlay();
}

void AOpenable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOpenable, OpenableState);
}

void AOpenable::Interact(ACharacter* InteractCharacter, FText InteractionName)
{
	UE_LOG(LogTemp, Warning, TEXT("ADoorBase::Interact) InteractionName : %s"), *InteractionName.ToString());
	FString Interaction = InteractionName.ToString();
	if (Interaction.Equals("Open"))
	{
		ServerRPC_Open(InteractCharacter);
	}
	else if (Interaction.Equals("Close"))
	{
		ServerRPC_Close();
	}
	else if (Interaction.Equals("Unlock"))
	{
		ServerRPC_Unlock();
	}
}

const TArray<FText>& AOpenable::GetInteractionNameArray()
{
	InteractionNameArray.Empty();
	switch (OpenableState) {
	case EOpenableState::OPEN:
		InteractionNameArray.Add(FText::FromName("Close"));
		break;
	case EOpenableState::UNLOCKED:
	case EOpenableState::CLOSE:
		InteractionNameArray.Add(FText::FromName("Open"));
		break;
	case EOpenableState::LOCKED:
		{
			// Todo : Check Key
			/*
			 * UInventoryComponent* InventoryComponent = InteractCharacter->GetComponentByClass<UInventoryComponent>()
			 *
			 *
			 *
			 *
			 *
			if (InventoryComponent->FindItem(KeyName))
			{
				InteractionNameArray.Add(FText::FromName("Use key"));
			}
			if (InventoryComponent->FindType(EItemType::Grenade))
			{
				InteractionNameArray.Add(FText::FromName("Use Grenade"));
			}
			*/
			
		}
		break;
	case EOpenableState::NONE:
		break;
	case EOpenableState::SIZE:
		break;
	}
	return InteractionNameArray;
}

FText AOpenable::GetActorName() const
{
	return FText::FromName("Openable");
}

void AOpenable::ServerRPC_Open_Implementation(ACharacter* InteractCharacter)
{
	if (OpenableState == EOpenableState::OPEN || OpenableState == EOpenableState::LOCKED)
    {
		return;
    }
	
    OpenableState = EOpenableState::OPEN;
	OnRep_OpenableState();
}

void AOpenable::ServerRPC_Close_Implementation()
{
	if (OpenableState == EOpenableState::CLOSE)
	{
		return;
	}
	
	OpenableState = EOpenableState::CLOSE;
	OnRep_OpenableState();
}

void AOpenable::ServerRPC_Unlock_Implementation()
{
	if (EOpenableState::LOCKED != OpenableState)
	{
		return;
	}
	
	// Todo : Inventory->UseItem(KeyName);
	
	OpenableState = EOpenableState::UNLOCKED;
	OnRep_OpenableState();
}

void AOpenable::Open()
{
}

void AOpenable::Close()
{
}

void AOpenable::Unlock()
{
}

void AOpenable::OnRep_OpenableState()
{
	switch (OpenableState)
    {
	case EOpenableState::NONE:
		break;
    case EOpenableState::OPEN:
    	Open();
    	break;
    case EOpenableState::CLOSE:
    	Close();
    	break;
    case EOpenableState::LOCKED:
    	break;
    case EOpenableState::UNLOCKED:
    	Unlock();
	    break;
    case EOpenableState::SIZE:
	    break;
    }
    	
	OnInteractActorChanged.ExecuteIfBound(this);
}

// Called every frame
void AOpenable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

