// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/CharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Component/WeaponComponent.h"
#include "GameFramework/Character.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		WeaponComponent = PlayerCharacter->GetComponentByClass<UWeaponComponent>();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerCharacter)
	{
		MoveSpeed = PlayerCharacter->GetVelocity().Length();
		MoveDirection = UKismetAnimationLibrary::CalculateDirection(PlayerCharacter->GetVelocity(), PlayerCharacter->GetActorRotation());
		bIsCrouched = PlayerCharacter->bIsCrouched;

		PitchAngle = FMath::Clamp(-PlayerCharacter->GetBaseAimRotation().GetNormalized().Pitch, -60.f, 60.f);
	}
}

void UCharacterAnimInstance::AnimNotify_FireBullet()
{
	WeaponComponent->FireBullet();
}