// Copyright James Jenkins. All Rights Reserved.

#include "Interactables/BaseInteractable.h"

ABaseInteractable::ABaseInteractable() : Super() {
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add("Interact");

	interactBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	RootComponent = interactBox;
	interactBox->SetHiddenInGame(true);
	interactBox->SetCollisionProfileName(TEXT("BlockAll"));
	interactBox->SetEnableGravity(false);
}
