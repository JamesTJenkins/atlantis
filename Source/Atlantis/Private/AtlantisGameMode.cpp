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

AAtlantisGameMode::AAtlantisGameMode() : Super() {
	static ConstructorHelpers::FClassFinder<AAtlantisCharacter> bodyguardCharacterClass(TEXT("/Game/Atlantis/Blueprints/BPC_Bodyguard"));
	static ConstructorHelpers::FClassFinder<AAtlantisCharacter> researcherCharacterClass(TEXT("/Game/Atlantis/Blueprints/RPC_Researcher"));
	bodyguard = bodyguardCharacterClass.Class;
	researcher = researcherCharacterClass.Class;
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
	// Choose a player start
	APlayerStart* foundPlayerStart = nullptr;
	UClass* pawnClass = GetDefaultPawnClassForController(player);
	APawn* pawnToFit = pawnClass ? pawnClass->GetDefaultObject<APawn>() : nullptr;
	TArray<APlayerStart*> unOccupiedStartPoints;
	TArray<APlayerStart*> occupiedStartPoints;
	UWorld* World = GetWorld();
	for(TActorIterator<APlayerStart> it(World); it; ++it) {
		APlayerStart* playerStart = *it;

		if(playerStart->IsA<APlayerStartPIE>()) {
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			foundPlayerStart = playerStart;
			break;
		} else {
			FVector ActorLocation = playerStart->GetActorLocation();
			const FRotator ActorRotation = playerStart->GetActorRotation();
			if(!World->EncroachingBlockingGeometry(pawnToFit, ActorLocation, ActorRotation)) {
				unOccupiedStartPoints.Add(playerStart);
			} else if(World->FindTeleportSpot(pawnToFit, ActorLocation, ActorRotation)) {
				occupiedStartPoints.Add(playerStart);
			}
		}
	}
	if(foundPlayerStart == nullptr) {
		if(unOccupiedStartPoints.Num() > 0) {
			foundPlayerStart = unOccupiedStartPoints[FMath::RandRange(0, unOccupiedStartPoints.Num() - 1)];
		} else if(occupiedStartPoints.Num() > 0) {
			foundPlayerStart = occupiedStartPoints[FMath::RandRange(0, occupiedStartPoints.Num() - 1)];
		}
	}
	return foundPlayerStart;
}

void AAtlantisGameMode::RestartPlayer(AController* newPlayer) {
	if(newPlayer == nullptr || newPlayer->IsPendingKillPending()) {
		return;
	}

	AActor* start = FindPlayerStart(newPlayer);

	// Until player selection is added this will do
	if (playerControllers.Num() == 1) {
		DefaultPawnClass = bodyguard;	// I could just make a proper spawner but this is far simpler
	} else {
		DefaultPawnClass = researcher;
	}

	RestartPlayerAtPlayerStart(newPlayer, start);
}
