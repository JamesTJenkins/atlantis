// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisPlayerState.h"
#include "AtlantisGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

#define MAIN_LEVEL_NAME FString("/Game/FirstPerson/Maps/FirstPersonMap")

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

	playerReady = false;
}

void AAtlantisPlayerState::SetPlayerRole(EPlayerRole newRole) {
	if (GetLocalRole() == ROLE_Authority) {
		playerRole = newRole;
		OnRep_PlayerRole();
	}
}

void AAtlantisPlayerState::SetPlayerReady(bool ready) {
	if (GetLocalRole() == ROLE_Authority) {
		playerReady = ready;
		OnRep_PlayerReady();
	}
}

void AAtlantisPlayerState::OnRep_PlayerRole() {
	OnRoleUpdate.Broadcast(this);
}

void AAtlantisPlayerState::OnRep_PlayerReady() {
	
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
		
		if (playerReady) {
			SetPlayerReady(false);
			UE_LOG(LogTemp, Log, TEXT("Player is no longer ready"));
		}

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

void AAtlantisPlayerState::RequestPlayerReady_Implementation(const bool ready) {
	if (ready && playerRole == EPlayerRole::None) {
		UE_LOG(LogTemp, Log, TEXT("Cant ready up as role not selected"));
		return;
	}
	
	SetPlayerReady(ready);
	UE_LOG(LogTemp, Log, TEXT("%hs"), ready ? "Player is ready" : "Player is not ready");

	if (ready) {
		AAtlantisGameMode* gamemode = Cast<AAtlantisGameMode>(GetWorld()->GetAuthGameMode());
		int readyCount = 0;
		for (APlayerController* pc : gamemode->playerControllers) {
			AAtlantisPlayerState* state = Cast<AAtlantisPlayerState>(pc->PlayerState);

			if (state->playerRole != EPlayerRole::None && state->playerReady) {
				readyCount++;
			}
		}

		if (readyCount >= 2) {
			gamemode->ChangeLevel(MAIN_LEVEL_NAME);
		}
	}
}

void AAtlantisPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAtlantisPlayerState, playerRole);
}


// Ready status is not copied over as that should be defaulted back to false
void AAtlantisPlayerState::CopyProperties(APlayerState* PlayerState) {
	Super::CopyProperties(PlayerState);

	if (IsValid(PlayerState)) {
		AAtlantisPlayerState* aState = Cast<AAtlantisPlayerState>(PlayerState);
		if (IsValid(aState)) {
			aState->SetPlayerRole(playerRole);
		}
	}
}

void AAtlantisPlayerState::OverrideWith(APlayerState* PlayerState) {
	Super::OverrideWith(PlayerState);

	if (IsValid(PlayerState)) {
		AAtlantisPlayerState* aState = Cast<AAtlantisPlayerState>(PlayerState);
		if (IsValid(aState)) {
			SetPlayerRole(aState->playerRole);
		}
	}
}
