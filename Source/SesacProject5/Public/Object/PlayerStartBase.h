// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerStartBase.generated.h"

class AExitBase;
class UEscapeComponent;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API APlayerStartBase : public APlayerStart
{
	GENERATED_BODY()

public:
	void SetExitArrayToComponent(UEscapeComponent* EscapeComponent);
	bool IsOccupied() const;
	APlayerStartBase* SetOccupied();
	void SetParent(APlayerStartBase* NewParent);

private:
	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<APlayerStartBase*> PlayerStartArray;
	APlayerStartBase* ParentPlayerStart;
	
	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<AExitBase*> ExitArray;
	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess))
	bool bIsOccupied = false;
};