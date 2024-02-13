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

USTRUCT()
struct FProcessTerminateState
{
	GENERATED_BODY()

	UPROPERTY()
	bool Status;

	long TerminationTime;

	FProcessTerminateState()
	{
		Status = false;
	}
};

USTRUCT()
struct FHealthCheckState
{
	GENERATED_BODY()

	UPROPERTY()
	bool Status;

	FHealthCheckState()
	{
		Status = false;
	}
};

UCLASS()
class SESACPROJECT5_API AGameLiftGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameLiftGameMode();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FStartGameSessionState StartGameSessionState;

	UPROPERTY()
	FUpdateGameSessionState UpdateGameSessionState;

	UPROPERTY()
	FProcessTerminateState ProcessTerminateState;

	UPROPERTY()
	FHealthCheckState HealthCheckState;
};
