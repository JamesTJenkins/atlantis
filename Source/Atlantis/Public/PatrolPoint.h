// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPoint.generated.h"

class USceneComponent;

UCLASS()
class ATLANTIS_API APatrolPoint : public AActor {
	GENERATED_BODY()
public:
	APatrolPoint();
private:
	USceneComponent* sceneComp;
};
