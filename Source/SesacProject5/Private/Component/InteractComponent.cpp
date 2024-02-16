// copyright by LootingEmployment


#include "Component/InteractComponent.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Interface/InteractInterface.h"
#include "UI/InGame/InteractWidget.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInteractComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &UInteractComponent::InteractAction);
	PlayerInputComponent->BindAction(IA_SelectInteraction, ETriggerEvent::Triggered, this, &UInteractComponent::SelectInteractionAction);
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	OwningCharacter = GetOwner<ACharacter>();
	CameraComponent = OwningCharacter->GetComponentByClass<UCameraComponent>();
	
	if (OwningCharacter->GetController() == nullptr ||  OwningCharacter->GetController()->IsLocalController() == false)
	{
		SetComponentTickEnabled(false);
	}
}

// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	FHitResult OutHit;
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + (CameraComponent->GetForwardVector() * TraceDistance);

	UWorld* World = GetWorld();
	if (World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility))
	{
		// UE_LOG(LogTemp, Log, TEXT("UInteractComponent::TickComponent) Trace Actor : %s"), *OutHit.GetActor()->GetActorNameOrLabel());

		if (InteractActor == OutHit.GetActor()) return;
		if (IInteractInterface* CurrentInteractActor =  Cast<IInteractInterface>(OutHit.GetActor()))
		{
			UE_LOG(LogTemp, Log, TEXT("UInteractComponent::TickComponent) Trace Actor : %s"), *OutHit.GetActor()->GetActorNameOrLabel());
			// Todo : Reset Interact Widget
			InteractActor = OutHit.GetActor();
			OnInteractActorChanged.ExecuteIfBound(CurrentInteractActor);
			return;
		}
	}

	if (InteractActor == nullptr) return;
	
	// Todo : Hide Interact Widget
	InteractActor = nullptr;
	OnInteractActorChanged.ExecuteIfBound(nullptr);
}

void UInteractComponent::SetInteractWidget(UInteractWidget* NewInteractWidget)
{
	InteractWidget = NewInteractWidget;
}

void UInteractComponent::ServerRPC_Interact_Implementation(AActor* InteractTarget, const FText& InteractionName)
{
	Cast<IInteractInterface>(InteractTarget)->Interact(OwningCharacter, InteractionName);
}

void UInteractComponent::InteractAction(const FInputActionValue& Value)
{
	if (InteractActor && InteractWidget)
	{
		ServerRPC_Interact(InteractActor, InteractWidget->GetInteractionName());	
	}
}

void UInteractComponent::SelectInteractionAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("UInteractComponent::SelectInteractionAction) Value : %f"), Value.Get<float>());

	if (InteractActor && InteractWidget)
	{
		if (Value.Get<float>() == 1.0f)
		{
			InteractWidget->SelectUp();
		}
		else
		{
			InteractWidget->SelectDown();
		}
	}
}