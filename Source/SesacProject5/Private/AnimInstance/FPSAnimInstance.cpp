// copyright by LootingEmployment


#include "AnimInstance/FPSAnimInstance.h"

#include "Character/CharacterBase.h"
#include "Component/HealthComponent.h"
#include "Component/MoveComponent.h"
#include "Component/WeaponComponent.h"
#include "GameFramework/Character.h"

void UFPSAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	OwningCharacter = Cast<ACharacterBase>(GetOwningActor());

	HealthComponent = OwningCharacter->GetComponentByClass<UHealthComponent>();
	
	if (auto MoveComponent = OwningCharacter->GetComponentByClass<UMoveComponent>())
	{
		MoveComponent->OnIsSprintChanged.AddUObject(this, &UFPSAnimInstance::UpdateIsSprint);
        MoveComponent->OnHandSwayFloatsChanged.BindUObject(this, &UFPSAnimInstance::UpdateHandSwayFloats);
		MoveComponent->OnLeanChanged.BindUObject(this, &UFPSAnimInstance::UpdateLeanBoolean);
	}

	WeaponComponent = OwningCharacter->GetComponentByClass<UWeaponComponent>();
	if (WeaponComponent)
	{
		WeaponComponent->OnIsAimingChanged.AddUObject(this, &UFPSAnimInstance::UpdateIsAiming);
		WeaponComponent->OnLeftHandIKChanged.BindUObject(this, &UFPSAnimInstance::UpdateLeftHandIK);
	}
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (bIsDead) return;
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwningCharacter == nullptr) return;
	
	bIsCrouch = OwningCharacter->bIsCrouched;

	if(HealthComponent)
	{
		Die(HealthComponent->IsDead());
	}

	LeanInterpolaction(DeltaSeconds);
}

void UFPSAnimInstance::Die(bool bNewIsDead)
{
	bIsDead = bNewIsDead;

	if (bIsDead)
	{
		UE_LOG(LogTemp, Warning, TEXT("UFPSAnimInstance::Die) %s "), *GetOwningActor()->GetActorNameOrLabel());
		if (DieMontageArray.Num() > 0)
		{
			Montage_Play(DieMontageArray[FMath::RandRange(0, DieMontageArray.Num()-1)]);
		}
	}
}

void UFPSAnimInstance::AnimNotify_OnDieEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("UFPSAnimInstance::AnimNotify_OnDieEnd) %s"), *GetOwningActor()->GetActorNameOrLabel());
	OwningCharacter->DieEnd();
}

void UFPSAnimInstance::AnimNotify_OnReloadEnd()
{
	SetLeftHandIKAlpha(1.f);

	WeaponComponent->OnReloadComplete();
}

void UFPSAnimInstance::UpdateIsSprint(bool bNewIsSprint)
{
	bIsSprint = bNewIsSprint;
}

void UFPSAnimInstance::UpdateIsAiming(bool bNewIsAiming)
{
	bIsAiming = bNewIsAiming;
}

void UFPSAnimInstance::UpdateHandSwayFloats(float NewSidMove, float NewMouseX, float NewMouseY)
{
	SideMove = NewSidMove;
	MouseX = NewMouseX;
	MouseY = NewMouseY;
}

void UFPSAnimInstance::UpdateLeftHandIK(FTransform NewLeftHandIK)
{
	LeftHandSocketTransform = NewLeftHandIK;
}

void UFPSAnimInstance::LeanInterpolaction(float DeltaSeconds)
{
	LeanAmount = FMath::FInterpTo(LeanAmount, LeanTarget, DeltaSeconds, LeanSpeed);

	LeanRotator.Pitch = LeanAmount * LeanMultiplier;
	// UE_LOG(LogTemp, Warning, TEXT("UFPSAnimInstance::UpdateLeanBoolean) Total Lean Amount : %f"), LeanAmount * LeanMultiplier);
}

void UFPSAnimInstance::UpdateLeanBoolean(bool bNewLeanLeft, bool bNewLeanRight)
{
	// UE_LOG(LogTemp, Warning, TEXT("UFPSAnimInstance::UpdateLeanBoolean) LeanLeft : %d, LeanRight : %d"), bNewLeanLeft, bNewLeanRight);
	if (bNewLeanLeft == bNewLeanRight)
	{
		LeanTarget = 0.f;
		return;
	}

	if (bNewLeanLeft)
	{
		LeanTarget = -1.f;
	}
	else
	{
		LeanTarget = 1.f;
	}
}

void UFPSAnimInstance::SetWallTargetValue(float NewWallTargetValue)
{
	WallValue = NewWallTargetValue;
}

void UFPSAnimInstance::SetLeftHandIKAlpha(float NewLeftHandIKAlpha)
{
	LeftHandIKAlpha = NewLeftHandIKAlpha;
}

void UFPSAnimInstance::ProcedualRecoil_Implementation(float Multiplier)
{
}