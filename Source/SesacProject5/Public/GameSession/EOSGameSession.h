// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "EOSGameSession.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API AEOSGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual bool ProcessAutoLogin() override;
	virtual void NotifyLogout(const APlayerController* ExitingPlayer) override;
	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;
	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void CreateSession(FName KeyName = "KeyName", FString KeyValue = "KeyValue");
	void StartSession();
	void EndSession();
	void DestroySession();

	void HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful);
	void HandleStartSessionCompleted(FName EOSSessionName, bool bWasSuccessful);
	void HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful);
	void HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccessful);
	
	void HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray< FUniqueNetIdRef >& PlayerId, bool bWasSuccessful);
	void HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray< FUniqueNetIdRef >& PlayerId, bool bWasSuccessful);
	
private:
	FDelegateHandle CreateSessionDelegateHandle;
	FDelegateHandle RegisterPlayerDelegateHandle;
	FDelegateHandle UnregisterPlayerDelegateHandle;
	FDelegateHandle StartSessionDelegateHandle;
	FDelegateHandle EndSessionDelegateHandle;
	FDelegateHandle DestroySessionDelegateHandle;
	
	const int32 MaxNumberOfPlayersInSession = 2;
	int32 NumberOfPlayersInSession = 0;
	bool bSessionExists;
};