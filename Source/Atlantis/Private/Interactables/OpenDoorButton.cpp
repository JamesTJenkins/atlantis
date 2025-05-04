// Copyright James Jenkins. All Rights Reserved.


#include "Interactables/OpenDoorButton.h"
#include "Components/StaticMeshComponent.h"

AOpenDoorButton::AOpenDoorButton() : Super() {
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WorldText"));
	staticMesh->AttachToComponent(interactBox, FAttachmentTransformRules::KeepRelativeTransform);
}

void AOpenDoorButton::OnInteract(AAtlantisCharacter* playerCharacter) {
	// TODO: implement animations
	door->Destroy();
}