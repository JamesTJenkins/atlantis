// Copyright James Jenkins. All Rights Reserved.

#include "LanguageTranslate.h"
#include "TimerManager.h"
#include "AtlantisPlayerController.h"
#include "ResearcherPlayerCharacter.h"

ALanguageTranslate::ALanguageTranslate() : Super() {
	PrimaryActorTick.bCanEverTick = false;

	worldText = CreateDefaultSubobject<UText3DComponent>(TEXT("WorldText"));
	worldText->SetupAttachment(RootComponent);
	worldText->SetHorizontalAlignment(EText3DHorizontalTextAlignment::Center);
	worldText->SetVerticalAlignment(EText3DVerticalTextAlignment::Center);

	Update3DText();
}

void ALanguageTranslate::SetNormalText(const FText& newText) {
	normalText = newText;
	Update3DText();
}

void ALanguageTranslate::Update3DText() {
	worldText->SetText(normalText);

	UWorld* world = GetWorld();
	if (world != nullptr) {
		// The bounds dont update immediately even if call rebuild before this so have to wait for next tick but
		// one issue I have found is the autosizing will not work if scene is not set to realtime.
		// Might look at making an editor tool to make a button that manually call this instead
		world->GetTimerManager().SetTimerForNextTick(this, &ALanguageTranslate::UpdateInteractionBox);
	}
}

void ALanguageTranslate::UpdateInteractionBox() {
	FVector origin;
	FVector extents;
	worldText->GetBounds(origin, extents);
	interactBox->SetBoxExtent(GetActorRotation().RotateVector(extents) * 1.1f);
}

void ALanguageTranslate::OnInteract(AAtlantisCharacter* playerCharacter) {
	if (AAtlantisPlayerController* controller = Cast<AAtlantisPlayerController>(playerCharacter->Controller)) {
		controller->ShowTranslatedTextToResearcher(this);
	}
}

#if WITH_EDITOR
void ALanguageTranslate::PostEditChangeProperty(struct FPropertyChangedEvent& propertyChangedEvent) {
	if (propertyChangedEvent.Property != nullptr) {
		if (propertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ALanguageTranslate, normalText)) {
			Update3DText();
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif