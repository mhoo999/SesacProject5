// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPSAnim_CharacterComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/EquipmentComponent.h"
#include "Component/EscapeComponent.h"
#include "Component/HealthComponent.h"
#include "Component/InteractComponent.h"
#include "Component/MoveComponent.h"
#include "Component/WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameInstance/EFSGameInstance.h"
#include "QuestSystem/QuestLogComponent.h"
#include "SaveData/QuestSaveData.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh());
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EscapeComponent = CreateDefaultSubobject<UEscapeComponent>(TEXT("EscapeComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));

	QuestLogComponent = CreateDefaultSubobject<UQuestLogComponent>(TEXT("QuestLogComponent"));

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	TeamId = FGenericTeamId(0);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		EscapeComponent->OnEscape.AddUObject(this, &ACharacterBase::Escape);
	}

	if (APlayerController* PC = GetController<APlayerController>())
	{
		if (IsLocallyControlled())
		{
			SpringArmComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Camera"));
			// SpringArmComponent->SetRelativeLocation(FVector(0, 0, 100));
			HealthComponent->OnIsDeadChanged.AddUObject(this, &ACharacterBase::Die);
			if (DefaultIMC)
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
				{
					Subsystem->AddMappingContext(DefaultIMC, 0);
				}
			}

			if (QuestLogComponent)
			{
				GetGameInstance<UEFSGameInstance>()->questData->LoadQuestLog(QuestLogComponent);
			}
		}
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// WeaponComponent->AimStartAction(FInputActionValue());
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (IsLocallyControlled() == false) return;
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		MoveComponent->SetupPlayerInputComponent(EnhancedInputComponent);
		InteractComponent->SetupPlayerInputComponent(EnhancedInputComponent);
		WeaponComponent->SetupPlayerInputComponent(EnhancedInputComponent);
	}
}

FVector ACharacterBase::GetCameraLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void ACharacterBase::Die(bool bIsDead)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DisableInput(GetController<APlayerController>());
	CameraComponent->PostProcessSettings.ColorSaturation = FVector::ZeroVector;
	CameraComponent->PostProcessSettings.bOverride_ColorSaturation = true;
}

void ACharacterBase::DieEnd()
{
	ServerRPC_MissingInAction();
}

void ACharacterBase::Escape()
{
	WeaponComponent->DestroyWeapon();
	EscapeComponent->ClientRPC_Escape();
}

void ACharacterBase::ServerRPC_MissingInAction_Implementation()
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->UnPossess();
		SetOwner(nullptr);
		PlayerController->ClientTravel("/Game/YMH/Level/ResultFailed_YMH", TRAVEL_Absolute);
	}
}
