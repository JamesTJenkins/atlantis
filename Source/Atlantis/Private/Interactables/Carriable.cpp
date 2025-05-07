// Copyright James Jenkins. All Rights Reserved.

#include "Interactables/Carriable.h"
#include "Components/StaticMeshComponent.h"

ACarriable::ACarriable() : Super() {
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	staticMesh->AttachToComponent(interactBox, FAttachmentTransformRules::KeepRelativeTransform);
	staticMesh->SetCollisionProfileName("NoCollision");

	SetPhysics(true);
}

void ACarriable::OnInteract(AAtlantisCharacter* playerCharacter) {
	playerCharacter->PickupCarriable(this);
}

void ACarriable::SetPhysics(bool enabled) {
	interactBox->SetSimulatePhysics(enabled);
	interactBox->SetEnableGravity(enabled);
}
