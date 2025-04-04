// Copyright Epic Games, Inc. All Rights Reserved.

#include "AtlantisPickUpComponent.h"

UAtlantisPickUpComponent::UAtlantisPickUpComponent() {
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UAtlantisPickUpComponent::BeginPlay() {
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UAtlantisPickUpComponent::OnSphereBeginOverlap);
}

void UAtlantisPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// Checking if it is a First Person Character overlapping
	AAtlantisCharacter* Character = Cast<AAtlantisCharacter>(OtherActor);
	if(Character != nullptr) {
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
