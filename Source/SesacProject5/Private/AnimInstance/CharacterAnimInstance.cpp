// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/CharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Component/MoveComponent.h"
#include "GameFramework/Character.h"

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// MoveSpeed = TryGetPawnOwner()->GetVelocity().Length();

	if (APawn* Pawn = TryGetPawnOwner())
	{
		MoveSpeed = Pawn->GetVelocity().Length();
		MoveDirection = UKismetAnimationLibrary::CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
		bIsCrouched = Cast<ACharacter>(Pawn)->bIsCrouched;

		// UE_LOG(LogTemp, Log, TEXT("UCharacterAnimInstance::NativeUpdateAnimation) MoveSpeed : %f"), MoveSpeed);
	}
}
