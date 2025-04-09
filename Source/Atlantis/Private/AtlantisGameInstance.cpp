// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include <Online\OnlineSessionNames.h>
#include "OnlineSubsystemUtils.h"

UAtlantisGameInstance::UAtlantisGameInstance() : Super() {

}

// TODO: update to use the lobbyname provided
void UAtlantisGameInstance::Host(const FName& lobbyName, bool LAN, bool privateLobby) {
	SessionSettings.bIsLANMatch = LAN;
	SessionSettings.bAllowInvites = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.NumPublicConnections = privateLobby ? 0 : 2;
	SessionSettings.NumPrivateConnections = privateLobby ? 2 : 0;
	SessionSettings.bShouldAdvertise = !privateLobby;
	SessionSettings.Set(SETTING_MAPNAME, FString("MainMenu"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
}

void UAtlantisGameInstance::JoinViaIP(const FString& address, const int port) {
	// Come back to this
	FURL url;
	url.Protocol = "tcp";
	url.Host = address;
	url.Port = port;

	GetFirstLocalPlayerController()->ClientTravel(url.ToString(), ETravelType::TRAVEL_Absolute, true);
}

void UAtlantisGameInstance::Join(const FBlueprintSessionResult& sessionToJoin) {
	SessionInterface->JoinSession(0, NAME_GameSession, sessionToJoin.OnlineResult);
}

void UAtlantisGameInstance::FindSessions(bool LAN) {
	SessionSearch->bIsLanQuery = LAN;
	SessionSearch->MaxSearchResults = 30;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UAtlantisGameInstance::Leave() {
	
}

void UAtlantisGameInstance::Init() {
	Super::Init();
	
	if(IOnlineSubsystem* subsystem = Online::GetSubsystem(GetWorld())) {
		SessionInterface = Online::GetSessionInterface(GetWorld());
		if(SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UAtlantisGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UAtlantisGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UAtlantisGameInstance::OnJoinSessionComplete);
		}
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
}

void UAtlantisGameInstance::OnCreateSessionComplete(FName sessionName, bool succeeded) {
	if(succeeded) {
		UE_LOG(LogTemp, Log, TEXT("Created Server"));
		// TODO: move to lobby screen
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to created Server"));
	}
}

void UAtlantisGameInstance::OnFindSessionComplete(bool succeeded) {
	if(succeeded) {
		UE_LOG(LogTemp, Log, TEXT("Completed session search"));
		TArray<FBlueprintSessionResult> sessionResults;

		for(FOnlineSessionSearchResult res : SessionSearch->SearchResults) {
			FBlueprintSessionResult bpResult;
			bpResult.OnlineResult = res;
			sessionResults.Add(bpResult);
		}

		OnFoundLobbies.Broadcast(sessionResults);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to find sessions"));
	}
}

void UAtlantisGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result) {
	// TODO: move to lobby screen
	UE_LOG(LogTemp, Warning, TEXT("Joined Server"));
}
