// Copyright James Jenkins. All Rights Reserved.


#include "Interactables/LockedDoor.h"
#include "Components/StaticMeshComponent.h"

ALockedDoor::ALockedDoor() : Super() {
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	staticMesh->AttachToComponent(interactBox, FAttachmentTransformRules::KeepRelativeTransform);
}

void ALockedDoor::OnInteract(AAtlantisCharacter* playerCharacter) {
	// TODO: implement animations/hud
	if(playerCharacter->CheckForId(requiredKeyId)) {
		Destroy();
	}
}