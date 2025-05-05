// Copyright James Jenkins. All Rights Reserved.

#include "Player/AtlantisPlayerController.h"
#include "HUD/ResearcherHUD.h"
#include "Player/ResearcherPlayerCharacter.h"
#include "Interactables/LanguageTranslate.h"
#include "TimerManager.h"

void AAtlantisPlayerController::ShowTranslatedTextToResearcher_Implementation(ALanguageTranslate* translate) {
	if(AResearcherHUD* researcherHUD = Cast<AResearcherHUD>(GetHUD())) {
		researcherHUD->OnTranslateStart.Broadcast(translate->normalText);
	}
	
	AResearcherPlayerCharacter* researcher = Cast<AResearcherPlayerCharacter>(GetPawn());
	researcher->currentInteractable = translate;
}

void AAtlantisPlayerController::ShowTempMessage_Implementation(const FText& text) {
	if(AAtlantisHUD* HUD = Cast<AAtlantisHUD>(GetHUD())) {
		HUD->OnTempMessageStart.Broadcast(text);

		if (tempMessageHandle.IsValid())
			tempMessageHandle.Invalidate();

		GetWorld()->GetTimerManager().SetTimer(tempMessageHandle, this, &AAtlantisPlayerController::TempMessageEnd, 3, false);
	}
}

void AAtlantisPlayerController::TempMessageEnd() {
	if(AAtlantisHUD* HUD = Cast<AAtlantisHUD>(GetHUD())) {
		HUD->OnTempMessageEnd.Broadcast();
	}
}
