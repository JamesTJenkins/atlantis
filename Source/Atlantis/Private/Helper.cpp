// Copyright James Jenkins. All Rights Reserved.

#include "Helper.h"
#include "FindSessionsCallbackProxy.h"
#include "AtlantisCharacter.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"

FString UHelper::GetSessionName(const FBlueprintSessionResult& Result) {
	FString name;
    Result.OnlineResult.Session.SessionSettings.Get(FName("SESSION_NAME"), name);
	return name;
}

bool UHelper::IsControlledByLocalPlayer(const UWorld* world, const AAtlantisCharacter* character) {
	APlayerController* playerController = Cast<APlayerController>(character->GetController());

	if(playerController) {
		FUniqueNetIdRepl characterNetId = playerController->PlayerState->GetUniqueId();
		FUniqueNetIdRepl localNetId = world->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId();

		return characterNetId.IsValid() && localNetId.IsValid() && (characterNetId == localNetId);
	}

	return false;
}
