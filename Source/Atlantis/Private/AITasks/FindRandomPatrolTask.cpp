// Copyright James Jenkins All Rights Reserved.

#include "AITasks/FindRandomPatrolTask.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolPoint.h"
#include "Enemy/BaseEnemyCharacter.h"
#include "AtlantisDefines.h"

UBTTask_FindRandomPatrolTask::UBTTask_FindRandomPatrolTask() : Super() {
	NodeName = "Find Random Patrol Point";
}

EBTNodeResult::Type UBTTask_FindRandomPatrolTask::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
	Super::ExecuteTask(ownerComp, nodeMemory);

	AAIController* controller = ownerComp.GetAIOwner();
	if(controller) {
		UBlackboardComponent* bb = controller->GetBlackboardComponent();
		if(bb) {
			ABaseEnemyCharacter* bec = Cast<ABaseEnemyCharacter>(controller->GetCharacter());
			if(bec && bec->patrolPoints.Num() > 0) {
				int index = FMath::RandRange(0, bec->patrolPoints.Num() - 1);

				// Just go to the next point if already at this one
				if (FVector::Distance(bec->GetActorLocation(), bec->patrolPoints[index]->GetActorLocation()) < 5)
					index = (index + 1) % (bec->patrolPoints.Num() - 1);

				bb->SetValueAsVector(PATROL_POINT_KEY, bec->patrolPoints[index]->GetActorLocation());

				FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	FinishLatentTask(ownerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}