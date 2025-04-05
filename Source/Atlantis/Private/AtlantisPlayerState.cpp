// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisPlayerState.h"
#include "AtlantisGameMode.h"

AAtlantisPlayerState::AAtlantisPlayerState() : Super() {
	InitPlayerState();
}

void AAtlantisPlayerState::InitPlayerState() {
	UWorld* world = GetWorld();
	if(world == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Cant get world for player state initalization"));
		return;
	}

	AGameModeBase* mode = world->GetAuthGameMode();
	if(mode == nullptr) {
		// This is for when game initially loads in editor the gamemode is nullptr
		// just return here so dont give bad info
		return;
	}

	AAtlantisGameMode* gamemode = Cast<AAtlantisGameMode>(world->GetAuthGameMode());
	if(gamemode == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Cant get gamemode is it set correctly?"));
		return;
	}

	// Until player selection is added this will do
	isBodyguard = gamemode->playerControllers.Num() == 1;
}
