// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "ScavBase.generated.h"

class UAIMumbleComponent;
class UAIMontageComponent;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API AScavBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AScavBase();

private:
	UPROPERTY(EditDefaultsOnly, Category="MySettings|FSM", meta=(AllowAbstract))
	UAIMontageComponent* AIMontageComp;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|FSM", meta=(AllowAbstract))
	UAIMumbleComponent* AIMumbleComp;

};
