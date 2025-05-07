// Copyright James Jenkins All Rights Reserved.

#include "PressurePlates/PressurePlate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player/AtlantisCharacter.h"
#include "Interactables/Carriable.h"

APressurePlate::APressurePlate() {
	PrimaryActorTick.bCanEverTick = false;

	pressurePlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PressurePlateMesh"));
	RootComponent = pressurePlateMesh;

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	triggerBox->SetupAttachment(pressurePlateMesh);
	triggerBox->SetHiddenInGame(true);
	triggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	triggerBox->SetEnableGravity(false);

	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnTriggerBeginOverlap);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnTriggerEndOverlap);

	pressed = 0;
}

void APressurePlate::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if(AAtlantisCharacter* character = Cast<AAtlantisCharacter>(OtherActor)) {
		pressed++;
	}

	if(ACarriable* carriable = Cast<ACarriable>(OtherActor)) {
		pressed++;
	}
}

void APressurePlate::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if(AAtlantisCharacter* character = Cast<AAtlantisCharacter>(OtherActor)) {
		pressed--;
	}

	if(ACarriable* carriable = Cast<ACarriable>(OtherActor)) {
		pressed--;
	}
}