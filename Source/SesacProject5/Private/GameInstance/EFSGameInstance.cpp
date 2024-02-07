// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/EFSGameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"


void UEFSGameInstance::LoginWithEOS(FString ID, FString Token, FString LoginType)
{
	if (IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineIdentityPtr IdentityPtrRef = SubsystemRef->GetIdentityInterface())
		{
			FOnlineAccountCredentials AccountCredentials;
			AccountCredentials.Id = ID;
			AccountCredentials.Token = Token;
			AccountCredentials.Type = LoginType;
			IdentityPtrRef->OnLoginCompleteDelegates->AddUObject(this, &UEFSGameInstance::OnLoginComplete);
			IdentityPtrRef->Login(0, AccountCredentials);
		}
	} 
}

FString UEFSGameInstance::GetPlayerUserName()
{
	if (IsPlayerLoggedIn())
	{
		if (IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineIdentityPtr IdentityPtrRef = SubsystemRef->GetIdentityInterface())
			{
				return IdentityPtrRef->GetPlayerNickname(0);
			}
		}
	}

	return FString();
}

bool UEFSGameInstance::IsPlayerLoggedIn()
{
	if (IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineIdentityPtr IdentityPtrRef = SubsystemRef->GetIdentityInterface())
		{
			return IdentityPtrRef->GetLoginStatus(0) == ELoginStatus::LoggedIn;
		}
	}

	return false;
}

void UEFSGameInstance::CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections)
{
	if (IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef == nullptr) return;

		FOnlineSessionSettings SessionCreationInfo;
		SessionCreationInfo.bIsDedicated = bIsDedicatedServer;
		SessionCreationInfo.bAllowInvites = true;
		SessionCreationInfo.bIsLANMatch = bIsLanServer;
		SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
		SessionCreationInfo.bUseLobbiesIfAvailable = false;
		SessionCreationInfo.bUsesPresence = false;
		SessionCreationInfo.bShouldAdvertise = true;
		
		SessionCreationInfo.Set(SEARCH_KEYWORDS, FString("RandomHi"), EOnlineDataAdvertisementType::ViaOnlineService);

		SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEFSGameInstance::OnCreateSessionComplete);
		SessionPtrRef->CreateSession(0, FName("MainSession"), SessionCreationInfo);
	}
}

void UEFSGameInstance::FindSessionAndJoin()
{
	if (IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef == nullptr) return;

		SessionSearch = MakeShareable(new FOnlineSessionSearch);
		// SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, false, EOnlineComparisonOp::Equals);
		SessionSearch->QuerySettings.SearchParams.Empty();
		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 20;
		SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEFSGameInstance::OnFindSessionComplete);
		SessionPtrRef->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UEFSGameInstance::JoinEOSSession() 
{
}

void UEFSGameInstance::DestroySession()
{
	if (IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef == nullptr) return;

		SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UEFSGameInstance::OnDestroySessionComplete);
		SessionPtrRef->DestroySession(FName("MainSession"));
	}
}

void UEFSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(LevelURL + FString("?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UEFSGameInstance::OnCreateSessionComplete) Failed to create session  : %s"), *SessionName.ToString());
	}
}

void UEFSGameInstance::OnFindSessionComplete(bool bWasSuccess)
{
	if (bWasSuccess)
	{
		if (IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(GetWorld()))
		{
			IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
			if (SessionPtrRef == nullptr) return;
			
			if (SessionSearch->SearchResults.Num() > 0 && SessionSearch->SearchResults[0].IsValid())
			{
				SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEFSGameInstance::OnJoinSessionComplete);
				SessionPtrRef->JoinSession(0, FName("MainSession"), SessionSearch->SearchResults[0]);	
			}
			else
			{
				CreateEOSSession(false, false, 10);
			}
		}
	}
	else
	{
		CreateEOSSession(false, false, 10);
	}
}

void UEFSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC == nullptr || PC->IsValidLowLevelFast() == false) return;

		FString JoinAddress;

		if (IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(GetWorld()))
		{
			IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
			if (SessionPtrRef == nullptr) return;

			if (SessionPtrRef->GetResolvedConnectString(SessionName, JoinAddress) == false || JoinAddress.IsEmpty()) return;
			
			PC->ClientTravel(JoinAddress, TRAVEL_Absolute);
		}
	}
}

void UEFSGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UEFSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEFSGameInstance::OnLoginComplete) Login Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UEFSGameInstance::OnLoginComplete) Login Falied Reason : %s"), *Error);
	}
}
