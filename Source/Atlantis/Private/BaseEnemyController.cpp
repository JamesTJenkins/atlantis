// Copyright James Jenkins All Rights Reserved.

#include "BaseEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AtlantisDefines.h"
#include "AtlantisCharacter.h"
#include "TimerManager.h"

ABaseEnemyController::ABaseEnemyController() : Super() {
	behaviourTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	sightConfig->SightRadius = 500;
	sightConfig->LoseSightRadius = 600;
	sightConfig->PeripheralVisionAngleDegrees = 90;
	sightConfig->SetMaxAge(5);

	PerceptionComponent->ConfigureSense(*sightConfig);
	PerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
}

void ABaseEnemyController::BeginPlay() {
	Super::BeginPlay();

	if(behaviorTree) {
		blackboardComp->InitializeBlackboard(*behaviorTree->BlackboardAsset);
		behaviourTreeComp->StartTree(*behaviorTree);
	}
}

void ABaseEnemyController::ActorsPerceptionUpdated(const TArray<AActor*>& updatedActors) {
	Super::ActorsPerceptionUpdated(updatedActors);

	for(AActor* actor : updatedActors) {
		if(actor->ActorHasTag(PLAYER_TAG)) {
			FActorPerceptionBlueprintInfo info;
			if (PerceptionComponent->GetActorsPerception(actor, info)) {
				AAtlantisCharacter* currentEnemy = Cast<AAtlantisCharacter>(blackboardComp->GetValueAsObject(ENEMY_KEY));
				bool isSighted = info.LastSensedStimuli.Num() > 0;

				if (currentEnemy != actor && isSighted) {
					// Check if new enemy player should be target or not
					if (!currentEnemy) {
						// Set as target enemy as there is none selected currently
						SetNewEnemyTarget(actor);
					} else {
						// Target whichever player is closer
						float currentEnemyDist = FVector::Distance(GetCharacter()->GetActorLocation(), currentEnemy->GetActorLocation());
						float newEnemyDist = FVector::Distance(GetCharacter()->GetActorLocation(), actor->GetActorLocation());
					
						if(newEnemyDist < currentEnemyDist) {
							SetNewEnemyTarget(actor);
						}
					}
				} else {
					blackboardComp->SetValueAsBool(HAS_LINE_OF_SIGHT_KEY, isSighted);

					if(isSighted) {
						CancelLostSightTimer();
					} else {
						StartLostSightTimer();
					}
				}
			}
		}
	}
}

void ABaseEnemyController::SetNewEnemyTarget(AActor* actor) {
	blackboardComp->SetValueAsObject(ENEMY_KEY, actor);
	blackboardComp->SetValueAsBool(HAS_LINE_OF_SIGHT_KEY, true);
	CancelLostSightTimer();
}

void ABaseEnemyController::StartLostSightTimer() {
	GetWorld()->GetTimerManager().SetTimer(lostLineOfSight, this, &ABaseEnemyController::LostSight, 2, false);
}

void ABaseEnemyController::CancelLostSightTimer() {
	if(lostLineOfSight.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(lostLineOfSight);
	}
}

void ABaseEnemyController::LostSight() {
	blackboardComp->SetValueAsObject(ENEMY_KEY, nullptr);
	blackboardComp->SetValueAsBool(HAS_LINE_OF_SIGHT_KEY, false);
}
