// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolPoint.h"
#include "BaseEnemyCharacter.generated.h"

// We dont replicate stats on the enemies as there is no intention of having any UI
// for the enemies so only the server needs to know

UCLASS()
class ATLANTIS_API ABaseEnemyCharacter : public ACharacter {
	GENERATED_BODY()
public:
	ABaseEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float health;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Patrol)
	TArray<APatrolPoint*> patrolPoints;
};
