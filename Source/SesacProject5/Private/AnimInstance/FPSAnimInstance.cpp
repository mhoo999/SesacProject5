// copyright by LootingEmployment


#include "AnimInstance/FPSAnimInstance.h"

#include "Character/CharacterBase.h"
#include "Component/HealthComponent.h"
#include "Component/MoveComponent.h"
#include "GameFramework/Character.h"

void UFPSAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	GetOwningActor()->GetComponentByClass<UHealthComponent>()->OnIsDeadChanged.AddUObject(this, &UFPSAnimInstance::Die);
	GetOwningActor()->GetComponentByClass<UMoveComponent>()->OnIsSprintChanged.AddUObject(this, &UFPSAnimInstance::UpdateIsSprint);
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (bIsDead) return;
	Super::NativeUpdateAnimation(DeltaSeconds);

	// bIsCrouch = Cast<ACharacter>(GetOwningActor())->bIsCrouched;

	if (ACharacter* Character = Cast<ACharacter>(GetOwningActor()))
	{
		bIsCrouch = Character->bIsCrouched;
	}
}

void UFPSAnimInstance::Die(bool bNewIsDead)
{
	bIsDead = bNewIsDead;

	if (bIsDead)
	{
		if (DieMontageArray.Num() > 0)
		{
			Montage_Play(DieMontageArray[FMath::RandRange(0, DieMontageArray.Num()-1)]);
		}
	}
}

void UFPSAnimInstance::AnimNotify_OnDieEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("UFPSAnimInstance::AnimNotify_OnDieEnd) %s"), *GetOwningActor()->GetActorNameOrLabel());
	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwningActor()))
	{
		if (Character->IsLocallyControlled())
		{
			Character->DieEnd();
		}
	}
}

void UFPSAnimInstance::UpdateIsSprint(bool bNewIsSprint)
{
	bIsSprint = bNewIsSprint;
}