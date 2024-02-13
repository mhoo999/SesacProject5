// copyright by LootingEmployment


#include "Component/FSMComponent.h"

UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UFSMComponent::ExecuteBehavior()
{	
	IFSMInterface::ExecuteBehavior();

	UE_LOG(LogTemp, Warning, TEXT("hello"));
}

