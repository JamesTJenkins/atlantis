// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyController.generated.h"

// We dont replicate stats on the enemies as there is no intention of having any UI
// for the enemies so only the server needs to know

class UBehaviorTree;
class UBlackboardData;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAISenseConfig_Sight;

UCLASS()
class ATLANTIS_API ABaseEnemyController : public AAIController {
	GENERATED_BODY()
public:
	ABaseEnemyController();

	UPROPERTY(EditDefaultsOnly, Category = AI)
	UBehaviorTree* behaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	UBlackboardData* blackboardData;
protected:
	virtual void BeginPlay() override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& updatedActors) override;
private:
	void SetNewEnemyTarget(AActor* actor);

	void StartLostSightTimer();
	void CancelLostSightTimer();
	void LostSight();

	UBehaviorTreeComponent* behaviourTreeComp;
	UBlackboardComponent* blackboardComp;

	// Perception component is on the AAIController by default so dont need to add here
	UPROPERTY(EditDefaultsOnly, Category = AI)
	UAISenseConfig_Sight* sightConfig;

	FTimerHandle lostLineOfSight;
};
