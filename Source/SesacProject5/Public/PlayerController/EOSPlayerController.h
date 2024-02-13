// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameFramework/PlayerController.h"
#include "EOSPlayerController.generated.h"

/**
 * 
 */
class FOnlineSessionSearch;
class FOnlineSessionSearchResult;
UCLASS()
class SESACPROJECT5_API AEOSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEOSPlayerController();
	
	void FindSessions(FName SearchKey = "KeyName", FString SearchValue = "KeyValue");
	void JoinSession();
	
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FDelegateHandle FindSessionsDelegateHandle;
	FDelegateHandle JoinSessionDelegateHandle;

	FOnlineSessionSearchResult* SessionToJoin;
	
	FString ConnectString;
protected:
	virtual void BeginPlay() override;

	void Login();

	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	FDelegateHandle LoginDelegateHandle;
};
