// Copyright James Jenkins All Rights Reserved.

#include "PatrolPoint.h"
#include "Components/SceneComponent.h"

APatrolPoint::APatrolPoint() : Super() {
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = sceneComp;

	bReplicates = false;
	SetActorHiddenInGame(true);
}