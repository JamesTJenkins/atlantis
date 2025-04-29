// Copyright James Jenkins All Rights Reserved.

#include "OxygenZones/OxygenZoneSphere.h"
#include <Components/SphereComponent.h>
#include "AtlantisCharacter.h"

AOxygenZoneSphere::AOxygenZoneSphere() : Super() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	triggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	triggerVolume->SetSphereRadius(50, false);
	triggerVolume->SetHiddenInGame(true);
	triggerVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	triggerVolume->SetEnableGravity(false);
	RootComponent = triggerVolume;

	triggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AOxygenZoneSphere::OnTriggerBeginOverlap);
	triggerVolume->OnComponentEndOverlap.AddDynamic(this, &AOxygenZoneSphere::OnTriggerEndOverlap);
}

void AOxygenZoneSphere::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if(AAtlantisCharacter* character = Cast<AAtlantisCharacter>(OtherActor)) {
		character->oxygenZoneCount++;
	}
}

void AOxygenZoneSphere::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if(AAtlantisCharacter* character = Cast<AAtlantisCharacter>(OtherActor)) {
		character->oxygenZoneCount--;
	}
}