// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AtlantisGameMode.generated.h"

UCLASS(minimalapi)
class AAtlantisGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	AAtlantisGameMode();

	virtual void PostLogin(APlayerController* newPlayer) override;
	virtual void Logout(AController* player) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* player) override;
	virtual void RestartPlayer(AController* newPlayer) override;

	TArray<APlayerController*> playerControllers;
private:
	TSubclassOf<APawn> bodyguard;
	TSubclassOf<APawn> researcher;
};
