// Copyright James Jenkins. All Rights Reserved.


#include "Interactables/LockedDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Player/AtlantisPlayerController.h"

ALockedDoor::ALockedDoor() : Super() {
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	staticMesh->AttachToComponent(interactBox, FAttachmentTransformRules::KeepRelativeTransform);

	bReplicates = true;
}

void ALockedDoor::OnInteract(AAtlantisCharacter* playerCharacter) {
	// TODO: implement animations/hud
	if(playerCharacter->CheckForId(requiredKeyId)) {
		Destroy();
	} else {
		if(AAtlantisPlayerController* controller = Cast<AAtlantisPlayerController>(playerCharacter->Controller)) {
			controller->ShowTempMessage(FText::FromString("Dont have the key"));
		}
	}
}