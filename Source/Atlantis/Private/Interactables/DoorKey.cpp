// Copyright James Jenkins. All Rights Reserved.


#include "Interactables/DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "Player/AtlantisPlayerController.h"

ADoorKey::ADoorKey() : Super() {
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	staticMesh->AttachToComponent(interactBox, FAttachmentTransformRules::KeepRelativeTransform);

	bReplicates = true;
}

void ADoorKey::OnInteract(AAtlantisCharacter* playerCharacter) {
	// TODO: implement HUD
	playerCharacter->AddKeyId(keyId);

	if(AAtlantisPlayerController* controller = Cast<AAtlantisPlayerController>(playerCharacter->Controller)) {
		FText formattedText = FText::Format(NSLOCTEXT("Namespace", "Key", "Collected {0}"), FText::FromName(keyId));
		controller->ShowTempMessage(formattedText);
	}

	Destroy();
}
