// Copyright James Jenkins All Rights Reserved.

#include "AtlantisGameMode.h"
#include "Player/AtlantisCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/PlayerStartPIE.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Player/BodyguardPlayerCharacter.h"
#include "Player/ResearcherPlayerCharacter.h"
#include "Player/AtlantisPlayerState.h"
#include "AtlantisGameInstance.h"
#include "AtlantisDefines.h"

AAtlantisGameMode::AAtlantisGameMode() : Super() {
	static ConstructorHelpers::FClassFinder<APawn> menuPawnClass(TEXT("/Game/Atlantis/Blueprints/P_MenuPawn"));
	static ConstructorHelpers::FClassFinder<AAtlantisCharacter> bodyguardCharacterClass(TEXT("/Game/Atlantis/Blueprints/BPC_Bodyguard"));
	static ConstructorHelpers::FClassFinder<AAtlantisCharacter> researcherCharacterClass(TEXT("/Game/Atlantis/Blueprints/RPC_Researcher"));
	static ConstructorHelpers::FClassFinder<APawn> prologueBodyguardPawnClass(TEXT("/Game/Atlantis/Blueprints/PP_Bodyguard"));
	static ConstructorHelpers::FClassFinder<APawn> prologueResearcherPawnClass(TEXT("/Game/Atlantis/Blueprints/PP_Researcher"));
	
	menuPawn = menuPawnClass.Class;
	bodyguard = bodyguardCharacterClass.Class;
	researcher = researcherCharacterClass.Class;
	prologueBodyguard = prologueBodyguardPawnClass.Class;
	prologueResearcher = prologueResearcherPawnClass.Class;

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

		if(state->playerRole == EPlayerRole::Bodyguard && playerStart->PlayerStartTag == BODYGUARD_TAG) {
			return playerStart;
		} else if (state->playerRole == EPlayerRole::Researcher && playerStart->PlayerStartTag == RESEARCHER_TAG) {
			return playerStart;
		} else if(state->playerRole == EPlayerRole::None) {
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
	AAtlantisPlayerState* state = newPlayer->GetPlayerState<AAtlantisPlayerState>();

	// I could just make a proper spawner but this is far simpler
	bool inPrologue = GetWorld()->GetMapName().Contains("Prologue");
	if (state->playerRole == EPlayerRole::Bodyguard) {
		DefaultPawnClass = inPrologue ? prologueBodyguard : bodyguard;
	} else if (state->playerRole == EPlayerRole::Researcher) {
		DefaultPawnClass = inPrologue ? prologueResearcher : researcher;
	} else {
		DefaultPawnClass = menuPawn;
	}

	RestartPlayerAtPlayerStart(newPlayer, start);
}
