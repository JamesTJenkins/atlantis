// Copyright James Jenkins All Rights Reserved.

#include "OxygenZone.h"
#include <Components\BoxComponent.h>
#include "AtlantisCharacter.h"

AOxygenZone::AOxygenZone() : Super() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	triggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	triggerVolume->SetBoxExtent(FVector(100, 100, 100));
	triggerVolume->SetHiddenInGame(true);
	triggerVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	triggerVolume->SetEnableGravity(false);
	RootComponent = triggerVolume;

	triggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AOxygenZone::OnTriggerBeginOverlap);
	triggerVolume->OnComponentEndOverlap.AddDynamic(this, &AOxygenZone::OnTriggerEndOverlap);
}

void AOxygenZone::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if(AAtlantisCharacter* character = Cast<AAtlantisCharacter>(OtherActor)) {
		character->oxygenZoneCount++;
	}
}

void AOxygenZone::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if(AAtlantisCharacter* character = Cast<AAtlantisCharacter>(OtherActor)) {
		character->oxygenZoneCount--;
	}
}