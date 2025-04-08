// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UAtlantisGameInstance::UAtlantisGameInstance() : Super() {

}

void UAtlantisGameInstance::Host(const FName& lobbyName, bool LAN, bool privateLobby) {
	FOnlineSessionSettings settings;
	settings.bIsLANMatch = LAN;
	settings.bAllowInvites = true;
	settings.bAllowJoinInProgress = false;
	settings.bIsDedicated = false;
	settings.bUsesPresence = true;
	settings.NumPublicConnections = privateLobby ? 0 : 2;
	settings.NumPrivateConnections = privateLobby ? 2 : 0;
	settings.bShouldAdvertise = !privateLobby;
	//settings.Set(lobbyName, FString("Atlantis Session"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, FName("Atlantis Session"), settings);
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
	SessionInterface->JoinSession(0, FName("Atlantis Session"), sessionToJoin.OnlineResult);
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

	if(IOnlineSubsystem* subsystem = IOnlineSubsystem::Get()) {
		SessionInterface = subsystem->GetSessionInterface();
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
