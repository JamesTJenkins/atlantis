// Copyright James Jenkins All Rights Reserved.

#include "PatrolPoint.h"
#include "Components/SceneComponent.h"

#if WITH_EDITOR
#include "Components/SphereComponent.h"
#endif

APatrolPoint::APatrolPoint() : Super() {
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = sceneComp;

#if WITH_EDITOR
	debugSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DebugSphere"));
	debugSphere->SetSphereRadius(35.0f, false);
	debugSphere->SetVisibility(true);
	debugSphere->bHiddenInGame = true;
	debugSphere->ShapeColor = FColor::Blue;
	debugSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	debugSphere->AttachToComponent(sceneComp, FAttachmentTransformRules::KeepRelativeTransform);
#endif

	bReplicates = false;
	SetActorHiddenInGame(true);
}