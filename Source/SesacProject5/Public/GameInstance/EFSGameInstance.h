// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "EFSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UEFSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable) 
	void LoginWithEOS();

	FString GetPlayerUsername() const;

	UFUNCTION(BlueprintCallable)
	bool IsPlayerLoggedIn() const;
	
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
	void CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections);
	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
	void FindSessionAndJoin();
	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
	void JoinEOSSession();
	
	void DestroyEOSSession();

	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionCompleted(bool bWasSuccessful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);
private:
	class IOnlineSubsystem* SubsystemRef;
	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe>  IdentityPtrRef;
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> SessionPtrRef;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FString ServerURL;
};