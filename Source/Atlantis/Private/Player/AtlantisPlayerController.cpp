// Copyright James Jenkins. All Rights Reserved.

#include "Player/AtlantisPlayerController.h"
#include "HUD/ResearcherHUD.h"
#include "Player/ResearcherPlayerCharacter.h"
#include "Interactables/LanguageTranslate.h"

void AAtlantisPlayerController::ShowTranslatedTextToResearcher_Implementation(ALanguageTranslate* translate) {
	if(AResearcherHUD* researcherHUD = Cast<AResearcherHUD>(GetHUD())) {
		researcherHUD->OnTranslateStart.Broadcast(translate->normalText);
	}
	
	AResearcherPlayerCharacter* researcher = Cast<AResearcherPlayerCharacter>(GetPawn());
	researcher->currentInteractable = translate;
}
