// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPoint.generated.h"

class USceneComponent;

#if WITH_EDITOR
class USphereComponent;
#endif

UCLASS()
class ATLANTIS_API APatrolPoint : public AActor {
	GENERATED_BODY()
public:
	APatrolPoint();
private:
	USceneComponent* sceneComp;

#if WITH_EDITOR
	// This is just to assist placement in editor
	USphereComponent* debugSphere;
#endif
};
