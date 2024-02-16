// copyright by LootingEmployment


#include "Object/DoorBase.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ADoorBase::ADoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DoorFrameMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMeshComponent"));
	SetRootComponent(DoorFrameMeshComponent);
	DoorFrameMeshComponent->SetCollisionProfileName(TEXT("Pawn"));
	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshComponent"));
	DoorMeshComponent->SetupAttachment(DoorFrameMeshComponent);
	DoorMeshComponent->SetCollisionProfileName(TEXT("Pawn"));
	DoorMeshComponent->SetCollisionProfileName(TEXT("Custom"));
	DoorFrameMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

// Called when the game starts or when spawned
void ADoorBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}
}

void ADoorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoorBase, DoorState);
}

// Called every frame
void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorBase::Interact(ACharacter* InteractCharacter, FText InteractionName)
{
	UE_LOG(LogTemp, Warning, TEXT("ADoorBase::Interact) InteractionName : %s"), *InteractionName.ToString());
	FString Interaction = InteractionName.ToString();
	if (Interaction.Equals("Open"))
	{
		Open();
	}
	else if (Interaction.Equals("Close"))
	{
		Close();
	}
}

const TArray<FText>& ADoorBase::GetInteractionNameArray()
{
	InteractionNameArray.Empty();
	switch (DoorState) {
	case EDoorState::OPEN:
		InteractionNameArray.Add(FText::FromName("Close"));
		break;
	case EDoorState::CLOSE:
		InteractionNameArray.Add(FText::FromName("Open"));
		break;
	case EDoorState::LOCKED:
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
	}
	if (DoorState != EDoorState::OPEN)
	{
		InteractionNameArray.Add(FText::FromName("Kick"));
	}
	
	return InteractionNameArray;
}

FText ADoorBase::GetActorName() const
{
	return FText::FromName("Door");
}

void ADoorBase::Open()
{
	if (HasAuthority())
	{
		// State is already Open -> return
        if (DoorState == EDoorState::OPEN)
        {
        	return;	
        }
        DoorState = EDoorState::OPEN;
	}
	DoorMeshComponent->SetRelativeRotation(FRotator(0, 90, 0));
}

void ADoorBase::Close()
{
	if (HasAuthority())
	{
		// State is already Open -> return
		if (DoorState == EDoorState::CLOSE)
		{
			return;	
		}
	
		DoorState = EDoorState::CLOSE;
	}
    DoorMeshComponent->SetRelativeRotation(FRotator(0, 0, 0));
}

void ADoorBase::OnRep_DoorState()
{
	switch (DoorState)
	{
	case EDoorState::OPEN:
		Open();
		break;
	case EDoorState::CLOSE:
		Close();
		break;
	case EDoorState::LOCKED:
		break;
	}
	
	OnInteractActorChanged.ExecuteIfBound(this);
}