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
}

// Called every frame
void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const TArray<FText>& ADoorBase::GetInteractionNameArray()
{
	Super::GetInteractionNameArray();
	
	if (OpenableState != EOpenableState::OPEN)
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
	UE_LOG(LogTemp, Warning, TEXT("ADoorBase::Open"));
	DoorMeshComponent->SetRelativeRotation(FRotator(0, 90, 0));
}

void ADoorBase::Close()
{
	DoorMeshComponent->SetRelativeRotation(FRotator(0, 0, 0));
}