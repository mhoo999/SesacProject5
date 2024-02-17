// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

UENUM()
enum class EDamageType : uint8
{
	NONE,
	Bullet,
	Bleeding,
	SIZE
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SESACPROJECT5_API IDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EDamageType GetDamageType() const = 0;
	virtual float GetDamage() const = 0;
	virtual AActor* GetIndicator() const = 0;
};