// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/CharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Component/HealthComponent.h"
#include "Component/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		WeaponComponent = PlayerCharacter->GetComponentByClass<UWeaponComponent>();
		HealthComponent = PlayerCharacter->GetComponentByClass<UHealthComponent>();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (bIsDead == true)
	{
		return;
	}
	
	if (PlayerCharacter)
	{
		if (HealthComponent->IsDead() == true)
		{
			if (APlayerController* PlayerController =  PlayerCharacter->GetController<APlayerController>())
			{
				if (PlayerController->IsLocalController())
				{
					PlayerCharacter->DisableInput(PlayerController);
				} 
			}
			UE_LOG(LogTemp, Warning, TEXT("UCharacterAnimInstance::NativeUpdateAnimation) Play Death Montage"));
			StopAllMontages(0.f);
			bIsDead = true;
			Montage_Play(DeathMontageArray[FMath::RandRange(0, DeathMontageArray.Num()-1)]);
		}
		
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
