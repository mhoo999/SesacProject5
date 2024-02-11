// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/EOSPlayerController.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"

AEOSPlayerController::AEOSPlayerController()
{
}

void AEOSPlayerController::FindSessions(FName SearchKey, FString SearchValue)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();

	Search->QuerySettings.SearchParams.Empty();

	Search->QuerySettings.Set(SearchKey, SearchValue, EOnlineComparisonOp::Equals);
	FindSessionsDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(
			this,
			&AEOSPlayerController::HandleFindSessionsCompleted,
			Search));

	UE_LOG(LogTemp, Log, TEXT("Finding session."));

	if (!Session->FindSessions(0, Search))
	{
		UE_LOG(LogTemp, Warning, TEXT("Find session failed"));
		Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
		FindSessionsDelegateHandle.Reset();
	}
}

void AEOSPlayerController::JoinSession()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	JoinSessionDelegateHandle =
		Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(
			this,
			&AEOSPlayerController::HandleJoinSessionCompleted));

	UE_LOG(LogTemp, Log, TEXT("Joining session."));
	if (!Session->JoinSession(0, "SessionName", *SessionToJoin))
	{
		UE_LOG(LogTemp, Warning, TEXT("Join session failed"));
		Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
		JoinSessionDelegateHandle.Reset();
	}
}

void AEOSPlayerController::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Found session."));

		for (auto SessionInSearchResult : Search->SearchResults)
		{
			// Typically you want to check if the session is valid before joining. There is a bug in the EOS OSS where IsValid() returns false when the session is created on a DS. 
			// Instead of customizing the engine for this project, we're simply not checking if the session is valid. The code below should go in this if statement once the bug is fixed. 
			/*
			if (SessionInSearchResult.IsValid()) 
			{
 
				
			}
			*/

			if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
			{
				SessionToJoin = &SessionInSearchResult;
			}

			break;
		}

		JoinSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find sessions."));
	}

	Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
	FindSessionsDelegateHandle.Reset();
}

void AEOSPlayerController::HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface(); 

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Joined session."));
		if (GEngine)
		{
			//ConnectString = "127.0.0.1:7777";
			FURL DedicatedServerURL(nullptr, *ConnectString, TRAVEL_Absolute);
			FString DedicatedServerJoinError;
			auto DedicatedServerJoinStatus = GEngine->Browse(GEngine->GetWorldContextFromWorldChecked(GetWorld()), DedicatedServerURL, DedicatedServerJoinError);
			if (DedicatedServerJoinStatus == EBrowseReturnVal::Failure)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to browse for dedicated server. Error is : %s"), *DedicatedServerJoinError);
			}
		}
	}
	Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
	JoinSessionDelegateHandle.Reset();
}

void AEOSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		Login();
	}
	
}

void AEOSPlayerController::Login()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();

	FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);

	if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
	{
		return;
	}

	LoginDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(0,
		FOnLoginCompleteDelegate::CreateUObject(
			this,
			&AEOSPlayerController::HandleLoginCompleted));

	FString AuthType;
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);
	if (!AuthType.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Logging into EOS..."));

		if (!Identity->AutoLogin(0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to login..."));
			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
			LoginDelegateHandle.Reset();
		}
	}
	else
	{
		FOnlineAccountCredentials Credentials("AccountPortal", "", "");

		UE_LOG(LogTemp, Log, TEXT("Logging into EOS..."));

		if (!Identity->Login(0, Credentials))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to login..."));
			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
			LoginDelegateHandle.Reset();
		}
	}
}

void AEOSPlayerController::HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
	const FString& Error)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	check(Subsystem);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Login callback completed!"));
		UE_LOG(LogTemp, Log, TEXT("Searching for a session..."));

		FindSessions();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EOS login failed."));
	}

	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
	LoginDelegateHandle.Reset();
}
