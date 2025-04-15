// Copyright James Jenkins. All Rights Reserved.

#include "ResearcherPlayerCharacter.h"
#include "ResearcherHUD.h"

AResearcherPlayerCharacter::AResearcherPlayerCharacter() : Super() {
	currentInteractable = nullptr;
}

void AResearcherPlayerCharacter::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	// Checks if still looking at the same language translate actor and when stop hide the translated UI
	if (currentInteractable) {
		if (ABaseInteractable* interactable = GetInteractable()) {
			if (currentInteractable != interactable) {
				currentInteractable = nullptr;
			}
		} else {
			currentInteractable = nullptr;
		}

		if (!currentInteractable) {
			if (AResearcherHUD* researcherHUD = Cast<AResearcherHUD>(GetLocalViewingPlayerController()->GetHUD())) {
				researcherHUD->OnTranslateComplete.Broadcast();
			}
		}
	}
}
