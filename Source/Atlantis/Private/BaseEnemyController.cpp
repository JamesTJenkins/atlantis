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
	sightConfig->SetMaxAge(1.5f);

	PerceptionComponent->ConfigureSense(*sightConfig);
	PerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());

	timeToLoseSight = 2;
	attackRange = 250;
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
				bool isSighted = info.LastSensedStimuli[info.LastSensedStimuli.Num() - 1].WasSuccessfullySensed();
				
				if (currentEnemy != actor && isSighted) {
					if (!currentEnemy) {
						SetNewEnemyTarget(actor);
					}
				} else {
					if(isSighted) {
						CancelLostSightTimer();
					} else {
						StartLostSightTimer();
					}
				}
			}
		} else {
			PerceptionComponent->ForgetActor(actor);
		}
	}
}

void ABaseEnemyController::SetNewEnemyTarget(AActor* actor) {
	blackboardComp->SetValueAsObject(ENEMY_KEY, actor);
	CancelLostSightTimer();
}

void ABaseEnemyController::StartLostSightTimer() {
	GetWorld()->GetTimerManager().SetTimer(lostLineOfSight, this, &ABaseEnemyController::LostSight, timeToLoseSight, false);
}

void ABaseEnemyController::CancelLostSightTimer() {
	if(lostLineOfSight.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(lostLineOfSight);
	}
}

void ABaseEnemyController::LostSight() {
	blackboardComp->SetValueAsObject(ENEMY_KEY, nullptr);
}
