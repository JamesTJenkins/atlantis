// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h"
#include "AtlantisGameInstance.generated.h"

UCLASS()
class ATLANTIS_API UAtlantisGameInstance : public UGameInstance {
	GENERATED_BODY()
public:	
	UAtlantisGameInstance();

	UPROPERTY(BlueprintAssignable, Category = Network)
	FBlueprintFindSessionsResultDelegate OnFoundLobbies;

	UFUNCTION(BlueprintCallable, Category = Network)
	void Host(const FName& lobbyName, bool LAN, bool privateLobby);

	UFUNCTION(BlueprintCallable, Category = Network)
	void JoinViaIP(const FString& address, const int port = 7777);

	UFUNCTION(BlueprintCallable, Category = Network)
	void Join(const FBlueprintSessionResult& sessionToJoin);

	UFUNCTION(BlueprintCallable, Category = Network)
	void FindSessions(bool LAN);

	UFUNCTION(BlueprintCallable, Category = Network)
	void Leave();
protected:
	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName sessionName, bool succeeded);
	virtual void OnFindSessionComplete(bool succeeded);
	virtual void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
	virtual void OnDestroySessionComplete(FName sessionName, bool succeeded);
private:
	TSharedPtr<FOnlineSessionSearch, ESPMode::ThreadSafe> SessionSearch;
	FOnlineSessionSettings SessionSettings;
	IOnlineSessionPtr SessionInterface;
};
