// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisPlayerState.h"
#include "AtlantisGameMode.h"
#include <Net/UnrealNetwork.h>

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

	if (world->GetMapName().Contains("MainMenu")) {
		playerRole = EPlayerRole::None;
	} else {
		// This exists more for in editor testing purposes and in theory should never happen in normal games
		playerRole = gamemode->playerControllers.Num() == 0 ? EPlayerRole::Bodyguard : EPlayerRole::Researcher;
	}

}

void AAtlantisPlayerState::SetPlayerRole(EPlayerRole newRole) {
	if (GetLocalRole() == ROLE_Authority) {
		playerRole = newRole;
		OnRep_PlayerRole();
	}
}

void AAtlantisPlayerState::OnRep_PlayerRole() {
	
}

void AAtlantisPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAtlantisPlayerState, playerRole);
}
