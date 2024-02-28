// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstance/EFSGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/MainMenuPlayerController.h"
#include "SaveData/QuestSaveData.h"


void UEFSGameInstance::Init()
{
	Super::Init();
	SubsystemRef = Online::GetSubsystem(GetWorld());
    check(SubsystemRef);
	
    IdentityPtrRef = SubsystemRef->GetIdentityInterface();
    check(IdentityPtrRef);
	
	SessionPtrRef = SubsystemRef->GetSessionInterface();
	check(SessionPtrRef);
	
	IdentityPtrRef->OnLoginCompleteDelegates->AddUObject(this, &UEFSGameInstance::OnLoginComplete);
	SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEFSGameInstance::OnCreateSessionCompleted);
	SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEFSGameInstance::OnFindSessionCompleted);
	SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UEFSGameInstance::OnDestroySessionCompleted);
	SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEFSGameInstance::OnJoinSessionCompleted);

	// Save quest data
	const FString slotName = "questdata";
	
	if (UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::Init) Load Success"));
		questData = Cast<UQuestSaveData>(UGameplayStatics::LoadGameFromSlot(slotName, 0));

		questData->PrintLog();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::Init) Load Fail"));
		questData = Cast<UQuestSaveData>(UGameplayStatics::CreateSaveGameObject(UQuestSaveData::StaticClass()));
		UGameplayStatics::SaveGameToSlot(questData, slotName, 0);
	}
}

void UEFSGameInstance::LoginWithEOS()
{
	FOnlineAccountCredentials Credentials;
	Credentials.Type = FString("AccountPortal");

	IdentityPtrRef->Login(0, Credentials);
}

FString UEFSGameInstance::GetPlayerUsername() const
{
	if (IsPlayerLoggedIn() == false) return FString();

	return IdentityPtrRef->GetPlayerNickname(0);
}

bool UEFSGameInstance::IsPlayerLoggedIn() const
{
	return IdentityPtrRef->GetLoginStatus(0) == ELoginStatus::LoggedIn;
}

void UEFSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	if (bWasSuccessful)
	{
		// UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::OnLoginComplete) Login Success"));

		GetWorld()->GetFirstPlayerController<AMainMenuPlayerController>()->OnLoginSuccess();
	}
	else
	{
		// Todo : Login Widget Button set enable 
		// UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::OnLoginComplete) Login Failed : %s"), *Error); 
	}
}

void UEFSGameInstance::CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections)
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsDedicated = bIsDedicatedServer;
	SessionSettings.bIsLANMatch = bIsLanServer;
	SessionSettings.bUseLobbiesIfAvailable = false;
	SessionSettings.NumPublicConnections = NumberOfPublicConnections;
	SessionSettings.bUsesPresence = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.Set(FName("ROOM_NAME"), FString("RoomName"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	Online::GetSubsystem(GetWorld())->GetSessionInterface()->CreateSession(0, "MainSession" , SessionSettings);
}

void UEFSGameInstance::FindSessionAndJoin()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.SearchParams.Empty();
	// SessionSearch->QuerySettings.Set(SEARCH_DEDICATED_ONLY, true, EOnlineComparisonOp::Equals);
	// SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	// SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, false, EOnlineComparisonOp::Equals);
	SessionSearch->MaxSearchResults = 20;
	SessionPtrRef->FindSessions(0, SessionSearch.ToSharedRef());
}

void UEFSGameInstance::JoinEOSSession()
{
	if (SessionSearch->SearchResults.Num() > 0)
	{
		SessionPtrRef->JoinSession(0, FName("MainSession"), SessionSearch->SearchResults[0]);
	}
}

void UEFSGameInstance::DestroyEOSSession()
{
	SessionPtrRef->DestroySession(FName("MainSession"));
}

void UEFSGameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(ServerURL);
	}
}

void UEFSGameInstance::OnFindSessionCompleted(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::OnFindSessionCompleted) Success to find session"));
		if (SessionSearch->SearchResults.Num() > 0)
		{
			// for (auto Iter : SessionSearch->SearchResults)
			// {
			// 	FString Value;
			// 	Iter.Session.SessionSettings.Get(FName("ROOM_NAME"), Value);
			// 	UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::OnFindSessionCompleted) Room Name : %s"), *Value);
			// }

			SessionPtrRef->JoinSession(0, FName("MainSession"), SessionSearch->SearchResults[0]);
		}
		else
		{
			GetWorld()->GetFirstPlayerController<AMainMenuPlayerController>()->OnJoinSessionFail();	
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::OnFindSessionCompleted) Fail to find session"));

		GetWorld()->GetFirstPlayerController<AMainMenuPlayerController>()->OnJoinSessionFail();
	}
}

void UEFSGameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Type::Success)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		FString JoinAddress;
		SessionPtrRef->GetResolvedConnectString(FName("MainSession"), JoinAddress);
		UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::OnJoinSessionCompleted) JoinAddress : %s"), *JoinAddress);
		if (JoinAddress.IsEmpty() == false)
		{
			PlayerController->ClientTravel(JoinAddress, TRAVEL_Absolute);
		}
	}
}

void UEFSGameInstance::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("UEFSGameInstance::OnDestroySessionCompleted"));
}