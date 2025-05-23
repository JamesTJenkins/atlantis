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

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = Network)
	void ChangeLevel(const FString& levelName);
protected:
	virtual void PostLogin(APlayerController* newPlayer) override;
	virtual void Logout(AController* player) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* player) override;
	virtual void RestartPlayer(AController* newPlayer) override;
public:
	TArray<APlayerController*> playerControllers;
private:
	TSubclassOf<APawn> menuPawn;
	TSubclassOf<APawn> bodyguard;
	TSubclassOf<APawn> researcher;
	TSubclassOf<APawn> prologueBodyguard;
	TSubclassOf<APawn> prologueResearcher;
};
