// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnPoint.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawnPoint : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UEscapeComponent;
class SESACPROJECT5_API ISpawnPoint
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetExitArrayToComponent(UEscapeComponent* EscapeComponent) = 0;
	virtual bool IsOccupied() const = 0;
	virtual ISpawnPoint* SetOccupied() = 0;
	virtual void SetParent(ISpawnPoint* NewParent) = 0;
};
