// copyright by LootingEmployment


#include "Component/InventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "UI/InventoryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, StorageArray);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetIsReplicated(true);

	for (int i = 0; i < StorageArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay"));
		StorageArray[i].IsOccupied.Init(false, StorageArray[i].Size.X * StorageArray[i].Size.Y);
	}
}

UInventoryWidget* UInventoryComponent::GetInventoryWidget()
{
	if (InventoryWidget == nullptr)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->SetInventoryComponent(this);
	}

	return InventoryWidget;
}

void UInventoryComponent::HideInventoryWidget()
{
	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent();
	}
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::OnRep_StorageArray()
{
	// Todo : ItemWidget Update
	OnStorageArrayChanged.ExecuteIfBound(StorageArray);
}

TArray<FStorage>& UInventoryComponent::GetStorageArray()
{
	return StorageArray;
}

void UInventoryComponent::AddItem(AItemBase* Item)
{
	for (int i = 0; i < StorageArray.Num(); ++i)
	{
		if (StorageArray[i].AddItem(Item) == true) break;
	}
}