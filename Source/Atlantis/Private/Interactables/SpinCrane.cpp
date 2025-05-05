// Copyright James Jenkins. All Rights Reserved.

#include "Interactables/SpinCrane.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AtlantisDefines.h"

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
	rotateRightHitbox->ComponentTags.Add(SHOOTABLE_TAG);

	rotateLeftHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("RotateLeftHitbox"));
	rotateLeftHitbox->SetupAttachment(craneLoad);
	rotateLeftHitbox->SetHiddenInGame(true);
	rotateLeftHitbox->SetCollisionProfileName(TEXT("BlockAll"));
	rotateLeftHitbox->SetEnableGravity(false);
	rotateLeftHitbox->ComponentTags.Add(SHOOTABLE_TAG);

	currentlyInteracting = false;
	raiseAmount = 50;
	lowerAmount = 50;
	minCraneYawRotation = 0;
	maxCraneYawRotation = 360;
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
		if(!IsRaised())
			return;

		FVector loc = craneLoad->GetRelativeLocation();
		loc.Z -= lowerAmount * deltaTime;

		if (loc.Z < lowestLoadZPos)
			loc.Z = lowestLoadZPos;

		craneLoad->SetRelativeLocation(loc);
	}
}

void ASpinCrane::OnHit(UPrimitiveComponent* hitComponent) {
	if (!IsRaised())
		return;

	if(hitComponent == rotateRightHitbox) {
		FRotator currentRotation = craneTop->GetRelativeRotation();
		currentRotation.Yaw -= 90;

		if (CanRotate(currentRotation.Yaw))
			craneTop->SetRelativeRotation(currentRotation);
	}

	if(hitComponent == rotateLeftHitbox) {
		FRotator currentRotation = craneTop->GetRelativeRotation();
		currentRotation.Yaw += 90;

		if(CanRotate(currentRotation.Yaw))
			craneTop->SetRelativeRotation(currentRotation);
	}
}

bool ASpinCrane::IsRaised() {
	return craneLoad->GetRelativeLocation().Z > lowestLoadZPos;
}

bool ASpinCrane::CanRotate(float yaw) {
	// If 360 max rotation always return true
	if (maxCraneYawRotation >= 360)
		return true;
		
	return yaw >= minCraneYawRotation && yaw <= maxCraneYawRotation;
}