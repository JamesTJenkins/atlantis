// Copyright James Jenkins. All Rights Reserved.

#include "LanguageTranslate.h"

ALanguageTranslate::ALanguageTranslate() {
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add("Translate");

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
	// TODO: Convert normal text to what the atlantis text is before setting it
	// probably will have to do a custom font
	worldText->SetText(normalText);
}

#if WITH_EDITOR
void ALanguageTranslate::PostEditChangeProperty(struct FPropertyChangedEvent& propertyChangedEvent) {
	if (propertyChangedEvent.Property != nullptr) {
		if (propertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ALanguageTranslate, normalText))
			Update3DText();
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif