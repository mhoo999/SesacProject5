// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/CharacterAnimInstance.h"

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// MoveSpeed = TryGetPawnOwner()->GetVelocity().Length();

	if (APawn* Pawn = TryGetPawnOwner())
	{
		MoveSpeed = Pawn->GetVelocity().Length();
	}

}