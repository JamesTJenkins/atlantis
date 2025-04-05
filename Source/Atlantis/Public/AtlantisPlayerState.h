// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AtlantisPlayerState.generated.h"

UCLASS()
class ATLANTIS_API AAtlantisPlayerState : public APlayerState {
	GENERATED_BODY()
public:
	AAtlantisPlayerState();

	void InitPlayerState();

	bool isBodyguard;
};
