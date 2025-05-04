// Copyright James Jenkins All Rights Reserved.

#include "OxygenZones/OxygenZoneBox.h"
#include <Components/BoxComponent.h>
#include "Player/AtlantisCharacter.h"

AOxygenZoneBox::AOxygenZoneBox() : Super() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	triggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	triggerVolume->SetBoxExtent(FVector(100, 100, 100));
	triggerVolume->SetHiddenInGame(true);
	triggerVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	triggerVolume->SetEnableGravity(false);
	RootComponent = triggerVolume;

	triggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AOxygenZoneBox::OnTriggerBeginOverlap);
	triggerVolume->OnComponentEndOverlap.AddDynamic(this, &AOxygenZoneBox::OnTriggerEndOverlap);
}

void AOxygenZoneBox::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if(AAtlantisCharacter* character = Cast<AAtlantisCharacter>(OtherActor)) {
		character->oxygenZoneCount++;
	}
}

void AOxygenZoneBox::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if(AAtlantisCharacter* character = Cast<AAtlantisCharacter>(OtherActor)) {
		character->oxygenZoneCount--;
	}
}