// Copyright James Jenkins. All Rights Reserved.


#include "Interactables/DoorKey.h"
#include "Components/StaticMeshComponent.h"

ADoorKey::ADoorKey() : Super() {
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	staticMesh->AttachToComponent(interactBox, FAttachmentTransformRules::KeepRelativeTransform);
}

void ADoorKey::OnInteract(AAtlantisCharacter* playerCharacter) {
	// TODO: implement HUD
	playerCharacter->AddKeyId(keyId);
}