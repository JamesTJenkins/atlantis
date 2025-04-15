// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AtlantisCharacter.h"
#include "ResearcherPlayerCharacter.generated.h"

UCLASS()
class ATLANTIS_API AResearcherPlayerCharacter : public AAtlantisCharacter {
	GENERATED_BODY()
public:
	AResearcherPlayerCharacter();

	ABaseInteractable* currentInteractable;
protected:
	virtual void Tick(float deltaTime) override;
};
