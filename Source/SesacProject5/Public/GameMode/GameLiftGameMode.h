// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameFramework/GameModeBase.h"
#include "GameLiftGameMode.generated.h"

/**
 * 
 */

USTRUCT()
struct FStartGameSessionState
{
	GENERATED_BODY()

	UPROPERTY()
	bool Status;

	FStartGameSessionState()
	{
		Status = false;
	}
};

USTRUCT()
struct FUpdateGameSessionState

{
	GENERATED_BODY()

	FUpdateGameSessionState()
	{}

};
UCLASS()
class SESACPROJECT5_API AGameLiftGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
};
