// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisPlayerController.h"
#include "ResearcherHUD.h"
#include "ResearcherPlayerCharacter.h"
#include "LanguageTranslate.h"

void AAtlantisPlayerController::ShowTranslatedTextToResearcher_Implementation(ALanguageTranslate* translate) {
	if(AResearcherHUD* researcherHUD = Cast<AResearcherHUD>(GetHUD())) {
		researcherHUD->OnTranslateStart.Broadcast(translate->normalText);
	}
	
	AResearcherPlayerCharacter* researcher = Cast<AResearcherPlayerCharacter>(GetPawn());
	researcher->currentInteractable = translate;
}
