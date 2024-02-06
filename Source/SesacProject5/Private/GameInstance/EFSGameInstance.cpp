// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/EFSGameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

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
