// Copyright James Jenkins All Rights Reserved.

#include "AtlantisGameMode.h"
#include "AtlantisCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/PlayerStartPIE.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "BodyguardPlayerCharacter.h"
#include "ResearcherPlayerCharacter.h"
#include "AtlantisPlayerState.h"
#include "AtlantisGameInstance.h"

AAtlantisGameMode::AAtlantisGameMode() : Super() {
	static ConstructorHelpers::FClassFinder<AAtlantisCharacter> bodyguardCharacterClass(TEXT("/Game/Atlantis/Blueprints/BPC_Bodyguard"));
	static ConstructorHelpers::FClassFinder<AAtlantisCharacter> researcherCharacterClass(TEXT("/Game/Atlantis/Blueprints/RPC_Researcher"));
	bodyguard = bodyguardCharacterClass.Class;
	researcher = researcherCharacterClass.Class;

	PlayerStateClass = AAtlantisPlayerState::StaticClass();

	bUseSeamlessTravel = true;
}

void AAtlantisGameMode::ChangeLevel_Implementation(const FString& levelName) {
	GetWorld()->SeamlessTravel(levelName);
	FString level = levelName;
	ProcessClientTravel(level, true, false);
}

void AAtlantisGameMode::PostLogin(APlayerController* newPlayer) {
	Super::PostLogin(newPlayer);

	playerControllers.Add(newPlayer);
}

void AAtlantisGameMode::Logout(AController* player) {
	Super::Logout(player);

	playerControllers.Remove(Cast<APlayerController>(player));
}

AActor* AAtlantisGameMode::ChoosePlayerStart_Implementation(AController* player) {
	APlayerStart* foundPlayerStart = nullptr;
	UClass* pawnClass = GetDefaultPawnClassForController(player);
	UWorld* world = GetWorld();
	AAtlantisPlayerState* state = player->GetPlayerState<AAtlantisPlayerState>();
	for(TActorIterator<APlayerStart> it(world); it; ++it) {
		APlayerStart* playerStart = *it;

		if(state->playerRole == EPlayerRole::Bodyguard && playerStart->PlayerStartTag.Compare("Bodyguard")) {
			return playerStart;
		} else if (state->playerRole == EPlayerRole::Researcher && playerStart->PlayerStartTag.Compare("Researcher")) {
			return playerStart;
		}
	}

	return foundPlayerStart;
}

void AAtlantisGameMode::RestartPlayer(AController* newPlayer) {
	if(newPlayer == nullptr || newPlayer->IsPendingKillPending()) {
		return;
	}

	AActor* start = FindPlayerStart(newPlayer);

	if (newPlayer->GetPlayerState<AAtlantisPlayerState>()->playerRole == EPlayerRole::Bodyguard) {
		DefaultPawnClass = bodyguard;	// I could just make a proper spawner but this is far simpler
	} else {
		DefaultPawnClass = researcher;
	}

	RestartPlayerAtPlayerStart(newPlayer, start);
}
