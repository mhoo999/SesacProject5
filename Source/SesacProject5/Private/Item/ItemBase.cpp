// copyright by LootingEmployment


#include "Item/ItemBase.h"

#include "Component/EquipmentComponent.h"
#include "Component/EscapeComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, ItemInstance);
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::Interact(ACharacter* InteractCharacter, FText InteractionName)
{
	// UE_LOG(LogTemp, Warning, TEXT("AItemBase::Interact) %s : %s"), *this->GetActorNameOrLabel(), *InteractionName.ToString());
	// Destroy();

	FString Interaction = InteractionName.ToString();

	if (Interaction.Equals("Get"))
	{
		InteractCharacter->GetComponentByClass<UEquipmentComponent>()->PutItem(this);
	}
}

const TArray<FText>& AItemBase::GetInteractionNameArray()
{
	return ItemData.InteractionNameArray;
}

FText AItemBase::GetActorName() const
{
	return ItemData.Name;
}

void AItemBase::PutToInventory(FStorage* Storage, FIntPoint StoragePosition)
{
	ItemInstance.StoragePosition = StoragePosition;

	MultiRPC_PutToInventory();
}

void AItemBase::PopFromInventory()
{
	MultiRPC_PopFromInventory();
}

FItemData AItemBase::GetItemData() const
{
	return ItemData;
}

FItemInstance AItemBase::GetItemInstance() const
{

	return ItemInstance;
}

void AItemBase::MultiRPC_PopFromInventory_Implementation()
{
	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AItemBase::MultiRPC_PutToInventory_Implementation()
{
	CollisionComponent->SetSimulatePhysics(false);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
 