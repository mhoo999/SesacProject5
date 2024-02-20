// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/EscapeComponent.h"
#include "Component/HealthComponent.h"
#include "Component/InteractComponent.h"
#include "Component/MoveComponent.h"
#include "Component/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);



	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	ArmMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMeshComponent"));
	ArmMeshComponent->SetupAttachment(CameraComponent);

	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EscapeComponent = CreateDefaultSubobject<UEscapeComponent>(TEXT("EscapeComponent"));

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	TeamId = FGenericTeamId(0);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = GetController<APlayerController>())
	{
		if (IsLocallyControlled() && DefaultIMC)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultIMC, 0);
			}
		}
	}

	if (GetController<APlayerController>() && IsLocallyControlled())
	{
		// Todo : First Person
		//SpringArmComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("eyes"));
		//SpringArmComponent->SetRelativeLocation(FVector(0, 0, 20));
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
