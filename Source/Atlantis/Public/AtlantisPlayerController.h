// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AtlantisPlayerController.generated.h"

class AResearcherPlayerCharacter;
class ALanguageTranslate;

UCLASS()
class ATLANTIS_API AAtlantisPlayerController : public APlayerController {
	GENERATED_BODY()
public:
	UFUNCTION(Client, Reliable)
	void ShowTranslatedTextToResearcher(ALanguageTranslate* translate);
};
