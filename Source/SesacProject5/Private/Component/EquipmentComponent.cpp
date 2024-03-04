// copyright by LootingEmployment


#include "Component/EquipmentComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	SetIsReplicated(true);

	// for (auto Iter : StorageArray)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("UEquipmentComponent::BeginPlay) Size X : %f, Size Y : %f"), Iter.Size.X , Iter.Size.Y);
	// 	Iter.IsOccupied.Init(false, Iter.Size.X * Iter.Size.Y);
	// }
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipmentComponent, EquipmentArray);
}

void UEquipmentComponent::OnRepEquipmentArray()
{
	
}

void UEquipmentComponent::OnRepStorageArray()
{
}

bool UEquipmentComponent::PutItem(AItemBase* Item)
{
	for (auto Iter : StorageArray)
	{
		if (Iter.PutItemToStorage(Item)) return true;
	}

	return false;
}

void UEquipmentComponent::ClientRPC_LootItem_Implementation(UInventoryComponent* TargetInventory)
{
	
}
