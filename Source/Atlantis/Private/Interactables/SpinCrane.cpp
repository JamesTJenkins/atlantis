// Copyright James Jenkins. All Rights Reserved.

#include "Interactables/SpinCrane.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

ASpinCrane::ASpinCrane() : Super() {
	PrimaryActorTick.bCanEverTick = true;

	craneBottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraneBottom"));
	RootComponent = craneBottom;

	interactBox->SetupAttachment(craneBottom);

	craneTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraneTop"));
	craneTop->SetupAttachment(craneBottom);

	craneLoad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraneLoad"));
	craneLoad->SetupAttachment(craneTop);

	rotateRightHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("RotateRightHitbox"));
	rotateRightHitbox->SetupAttachment(craneLoad);
	rotateRightHitbox->SetHiddenInGame(true);
	rotateRightHitbox->SetCollisionProfileName(TEXT("BlockAll"));
	rotateRightHitbox->SetEnableGravity(false);

	rotateLeftHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("RotateLeftHitbox"));
	rotateLeftHitbox->SetupAttachment(craneLoad);
	rotateLeftHitbox->SetHiddenInGame(true);
	rotateLeftHitbox->SetCollisionProfileName(TEXT("BlockAll"));
	rotateLeftHitbox->SetEnableGravity(false);

	currentlyInteracting = false;
	raiseAmount = 50;
	lowerAmount = 50;
}

void ASpinCrane::OnInteractHold(AAtlantisCharacter* playerCharacter, float deltaTime) {
	currentlyInteracting = true;

	if(craneLoad->GetRelativeLocation().Z >= highestLoadZPos)
		return;

	FVector loc = craneLoad->GetRelativeLocation();
	loc.Z += raiseAmount * deltaTime;

	if(loc.Z > highestLoadZPos)
		loc.Z = highestLoadZPos;

	craneLoad->SetRelativeLocation(loc);
}

void ASpinCrane::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	if(currentlyInteracting) {
		currentlyInteracting = false;
	} else {
		if(craneLoad->GetRelativeLocation().Z <= lowestLoadZPos)
			return;

		FVector loc = craneLoad->GetRelativeLocation();
		loc.Z -= lowerAmount * deltaTime;

		if (loc.Z < lowestLoadZPos)
			loc.Z = lowestLoadZPos;

		craneLoad->SetRelativeLocation(loc);
	}
}