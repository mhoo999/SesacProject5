// copyright by LootingEmployment


#include "Object/LootBox.h"

#include "Component/EquipmentComponent.h"
#include "Component/InventoryComponent.h"
#include "GameFramework/Character.h"
#include "Item/ItemBase.h"

// Sets default values
ALootBox::ALootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BoxMeshComponent");
	SetRootComponent(BoxMeshComponent);
	
	LidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("LidMeshComponent");
	LidMeshComponent->SetupAttachment(RootComponent);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void ALootBox::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALootBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FText ALootBox::GetActorName() const
{
	return FText::FromName("LootBox");
}

void ALootBox::ServerRPC_Open(ACharacter* InteractCharacter)
{
	Super::ServerRPC_Open(InteractCharacter);

	InteractCharacter->GetComponentByClass<UEquipmentComponent>()->ClientRPC_LootItem(InventoryComponent);
}

void ALootBox::Open()
{
	Super::Open();

	LidMeshComponent->SetRelativeRotation(FRotator(0,0,-10));
}

void ALootBox::Close()
{
	Super::Close();
	
	LidMeshComponent->SetRelativeRotation(FRotator(0,0,0));
}

void ALootBox::InitBox()
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ItemClassArray.Num() > 0)
	{
		UWorld* World = GetWorld();

		InventoryComponent->AddItem(World->SpawnActor<AItemBase>(ItemClassArray[0], GetActorLocation(), GetActorRotation(), Param));
		InventoryComponent->AddItem(World->SpawnActor<AItemBase>(ItemClassArray[0], GetActorLocation(), GetActorRotation(), Param));
		InventoryComponent->AddItem(World->SpawnActor<AItemBase>(ItemClassArray[0], GetActorLocation(), GetActorRotation(), Param));
	}
}