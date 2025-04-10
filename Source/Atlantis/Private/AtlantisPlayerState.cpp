// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisPlayerState.h"
#include "AtlantisGameMode.h"
#include <Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

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

	if (world->GetMapName().Contains("Lobby")) {
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
	OnRoleUpdate.Broadcast(this);
}

bool AAtlantisPlayerState::CheckIfRoleTaken(const EPlayerRole roleToCheck, FUniqueNetIdRepl& OutNetId) {
	if (roleToCheck == EPlayerRole::None)
		return false;

	AAtlantisGameMode* gamemode = Cast<AAtlantisGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gamemode == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Gamemode not found. CheckIfRoleTaken can only be ran on the server"));

	for(APlayerController* pc : gamemode->playerControllers) {
		AAtlantisPlayerState* state = Cast<AAtlantisPlayerState>(pc->PlayerState);
		if(state->playerRole == roleToCheck) {
			OutNetId = state->GetUniqueId();
			return true;
		}
	}

	OutNetId = nullptr;
	return false;
}

void AAtlantisPlayerState::RequestClaimRole_Implementation(const EPlayerRole roleToClaim) {
	FUniqueNetIdRepl id;
	bool roleTaken = CheckIfRoleTaken(roleToClaim, id);

	// Unclaim role
	if(roleTaken && id == GetUniqueId()) {
		SetPlayerRole(EPlayerRole::None);

		UE_LOG(LogTemp, Log, TEXT("Role unclaimed"));
		return;
	}

	// Try to claim new role
	if(!roleTaken && playerRole == EPlayerRole::None) {
		SetPlayerRole(roleToClaim);

		UE_LOG(LogTemp, Log, TEXT("Role claimed"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Role is already taken"));
}

void AAtlantisPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAtlantisPlayerState, playerRole);
}
