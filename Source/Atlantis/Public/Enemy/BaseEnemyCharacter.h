// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "PatrolPoint.h"
#include "BaseEnemyCharacter.generated.h"

// We dont replicate stats on the enemies as there is no intention of having any UI
// for the enemies so only the server needs to know

UCLASS()
class ATLANTIS_API ABaseEnemyCharacter : public ACharacter, public ICharacterInterface {
	GENERATED_BODY()
public:
	ABaseEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float damage;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Patrol)
	TArray<APatrolPoint*> patrolPoints;

	UFUNCTION()
	virtual void UpdateHealth(float value) override;
};
