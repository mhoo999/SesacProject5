// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MoveComponent.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	OwningCharacter = GetOwner<ACharacter>();
	check(OwningCharacter != nullptr && OwningCharacter->IsValidLowLevelFast());

	if (OwningCharacter->HasAuthority())
	{
		SetIsReplicated(true);
	}
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMoveComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMoveComponent, bIsCrouched);
}

void UMoveComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UMoveComponent::MoveAction);
	PlayerInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UMoveComponent::LookAction);
	PlayerInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &UMoveComponent::CrouchAction);
}

bool UMoveComponent::IsCrouched() const
{
	return bIsCrouched;
}

void UMoveComponent::MoveAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("UMoveComponent::MoveAction"));

	FVector2D Vector2DValue = Value.Get<FVector2D>();

	if (Vector2DValue == FVector2D::ZeroVector) return;

	FRotationMatrix RotationMatrix(OwningCharacter->GetControlRotation());

	OwningCharacter->AddMovementInput(RotationMatrix.GetScaledAxis(EAxis::X), Vector2DValue.Y * 300.f);
	OwningCharacter->AddMovementInput(RotationMatrix.GetScaledAxis(EAxis::Y), Vector2DValue.X * 300.f);
}

void UMoveComponent::LookAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("UMoveComponent::LookAction"));

	FVector2D Vector2DValue = Value.Get<FVector2D>();

	OwningCharacter->AddControllerPitchInput(-Vector2DValue.Y);
	OwningCharacter->AddControllerYawInput(Vector2DValue.X);
}

void UMoveComponent::CrouchAction(const FInputActionValue& Value)
{
	ServerRPC_Crouch();
}

void UMoveComponent::ServerRPC_Crouch_Implementation()
{
	bIsCrouched = !bIsCrouched;

	if (bIsCrouched)
	{
		
	}
	else
	{
		
	}
}