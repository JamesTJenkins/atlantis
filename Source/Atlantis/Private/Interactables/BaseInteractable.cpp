// Copyright James Jenkins. All Rights Reserved.

#include "Interactables/BaseInteractable.h"
#include "AtlantisDefines.h"

ABaseInteractable::ABaseInteractable() : Super() {
	PrimaryActorTick.bCanEverTick = false;

	interactBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	RootComponent = interactBox;
	interactBox->SetHiddenInGame(true);
	interactBox->SetCollisionProfileName(TEXT("BlockAll"));
	interactBox->SetEnableGravity(false);
	interactBox->ComponentTags.Add(INTERACT_TAG);
}
