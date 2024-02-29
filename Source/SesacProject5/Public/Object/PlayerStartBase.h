// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/SpawnPoint.h"
#include "PlayerStartBase.generated.h"

class AExitBase;
class UEscapeComponent;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API APlayerStartBase : public APlayerStart, public ISpawnPoint
{
	GENERATED_BODY()

public:
	virtual void SetExitArrayToComponent(UEscapeComponent* EscapeComponent) override;
	virtual bool IsOccupied() const override;
	virtual ISpawnPoint* SetOccupied() override;
	virtual void SetParent(ISpawnPoint* NewParent) override;

private:
	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<APlayerStartBase*> SpawnPointArray;
	ISpawnPoint* ParentSpawnPoint;
	
	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<AExitBase*> ExitArray;
	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess))
	bool bIsOccupied = false;
};