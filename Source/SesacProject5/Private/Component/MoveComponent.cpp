// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MoveComponent.h"

#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
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

	if (OwningCharacter->IsLocallyControlled())
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(SwayFloatTimerHandle, this, &UMoveComponent::SwayFloatTimerFunction, 0.015f, true);	
		}
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
	DOREPLIFETIME(UMoveComponent, bIsSprint);
}

void UMoveComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UMoveComponent::MoveAction);
	PlayerInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &UMoveComponent::MoveEndAction);
	PlayerInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UMoveComponent::LookAction);
	PlayerInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &UMoveComponent::CrouchAction);
	PlayerInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &UMoveComponent::SprintStartAction);
	PlayerInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &UMoveComponent::SprintEndAction);
	PlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UMoveComponent::JumpAction);
	PlayerInputComponent->BindAction(IA_LeanLeft, ETriggerEvent::Started, this, &UMoveComponent::LeanLeftStartAction);
	PlayerInputComponent->BindAction(IA_LeanLeft, ETriggerEvent::Completed, this, &UMoveComponent::LeanLeftEndAction);
	PlayerInputComponent->BindAction(IA_LeanRight, ETriggerEvent::Started, this, &UMoveComponent::LeanRightStartAction);
	PlayerInputComponent->BindAction(IA_LeanRight, ETriggerEvent::Completed, this, &UMoveComponent::LeanRightEndAction);
}

void UMoveComponent::StopSprint()
{
	SprintEndAction(FInputActionValue());
}

void UMoveComponent::MoveAction(const FInputActionValue& Value)
{
	// UE_LOG(LogTemp, Warning, TEXT("UMoveComponent::MoveAction"));

	FVector2D Vector2DValue = Value.Get<FVector2D>();

	SideMove = Vector2DValue.X;

	if (Vector2DValue == FVector2D::ZeroVector) return;

	FRotationMatrix RotationMatrix(FRotator(0, OwningCharacter->GetControlRotation().Yaw, 0));

	// UE_LOG(LogTemp, Warning, TEXT("UMoveComponent::MoveAction) X : %f, Y : %f"), Vector2DValue.X, Vector2DValue.Y);

	OwningCharacter->AddMovementInput(RotationMatrix.GetScaledAxis(EAxis::X), Vector2DValue.Y);
	OwningCharacter->AddMovementInput(RotationMatrix.GetScaledAxis(EAxis::Y), Vector2DValue.X);

	if (bIsSprint)
	{
		if (Vector2DValue.Y <= 0.f)
		{
			SprintEndAction(FInputActionValue());
		}
	}
}

void UMoveComponent::MoveEndAction(const FInputActionValue& Value)
{
	FVector2D Vector2DValue = Value.Get<FVector2D>();
	SideMove = Vector2DValue.X;
	
	bIsSprint = false;
}

void UMoveComponent::LookAction(const FInputActionValue& Value)
{
	// UE_LOG(LogTemp, Warning, TEXT("UMoveComponent::LookAction"));

	FVector2D Vector2DValue = Value.Get<FVector2D>();

	OwningCharacter->AddControllerPitchInput(-Vector2DValue.Y);
	OwningCharacter->AddControllerYawInput(Vector2DValue.X);
}

void UMoveComponent::CrouchAction(const FInputActionValue& Value)
{
	if (OwningCharacter->bIsCrouched)
	{
		OwningCharacter->UnCrouch();
	}
	else
	{
		OwningCharacter->Crouch();	
	}
}

void UMoveComponent::SprintStartAction(const FInputActionValue& Value)
{
	// Todo : To need to check Stemina from Status Component
	auto CharacterMovement = OwningCharacter->GetCharacterMovement();
	if (CharacterMovement->IsCrouching())
	{
		OwningCharacter->UnCrouch();
	}

	ServerRPC_StartSprint_Implementation();
	ServerRPC_StartSprint();
}

void UMoveComponent::SprintEndAction(const FInputActionValue& Value)
{
	ServerRPC_StopSprint_Implementation();
	ServerRPC_StopSprint();
}

void UMoveComponent::JumpAction(const FInputActionValue& Value)
{
	if (OwningCharacter->CanJump())
	{
		OwningCharacter->Jump();
	}
}

void UMoveComponent::LeanLeftStartAction(const FInputActionValue& Value)
{
}

void UMoveComponent::LeanLeftEndAction(const FInputActionValue& Value)
{
}

void UMoveComponent::LeanRightStartAction(const FInputActionValue& Value)
{
}

void UMoveComponent::LeanRightEndAction(const FInputActionValue& Value)
{
}

void UMoveComponent::ServerRPC_StartSprint_Implementation()
{
	// Todo : Check Sprint
	bIsSprint = true;
	OnRep_IsSprint();
}

void UMoveComponent::ServerRPC_StopSprint_Implementation()
{
	// Todo : Check Sprint
	bIsSprint = false;
	OnRep_IsSprint();
}

void UMoveComponent::OnRep_IsSprint()
{
	if (OwningCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UMoveComponent::OnRep_IsSprint) OwningCharacter is nullptr"));
		return;
	}
	if (bIsSprint)
	{
		OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	OnIsSprintChanged.Broadcast(bIsSprint);
}

void UMoveComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UMoveComponent::SwayFloatTimerFunction()
{
	if (APlayerController* PC = OwningCharacter->GetController<APlayerController>())
	{
		PC->GetMousePosition(MouseX, MouseY);
		OnHandSwayFloatsChanged.ExecuteIfBound(SideMove, MouseX, MouseY);
	}
}