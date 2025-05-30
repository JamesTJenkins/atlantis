// Copyright James Jenkins. All Rights Reserved.

#include "Interactables/Carriable.h"
#include "Components/StaticMeshComponent.h"

ACarriable::ACarriable() : Super() {
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	staticMesh->AttachToComponent(interactBox, FAttachmentTransformRules::KeepRelativeTransform);
	staticMesh->SetCollisionProfileName("NoCollision");
	
	interactBox->SetSimulatePhysics(true);
	interactBox->SetEnableGravity(true);

	bReplicates = true;
	SetReplicates(true);

	FRepMovement replMove;
	replMove.LinearVelocity = FVector::ZeroVector;
	replMove.AngularVelocity = FVector::ZeroVector;
	replMove.Location = FVector::ZeroVector;
	replMove.Rotation = FRotator::ZeroRotator;
	replMove.bSimulatedPhysicSleep = false;
	replMove.bRepPhysics = false;
	SetReplicatedMovement(replMove);

	interactBox->SetIsReplicated(true);
}

void ACarriable::OnInteract(AAtlantisCharacter* playerCharacter) {
	playerCharacter->PickupCarriable(this);
}

void ACarriable::EnableDampening(bool enabled) {
	if(enabled) {
		originalLinearDampening = interactBox->GetLinearDamping();
		originalAngularDampening = interactBox->GetAngularDamping();
		interactBox->SetLinearDamping(5);
		interactBox->SetAngularDamping(5);
	} else {
		interactBox->SetLinearDamping(originalLinearDampening);
		interactBox->SetAngularDamping(originalAngularDampening);
	}
}

void ACarriable::AddForce(FVector force) {
	interactBox->AddForce(force, NAME_None, true);
}
