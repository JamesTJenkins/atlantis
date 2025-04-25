// Copyright James Jenkins All Rights Reserved.

#include "AITasks/AttackTask.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BaseEnemyCharacter.h"
#include "AtlantisDefines.h"
#include "AtlantisCharacter.h"

UBTTask_AttackTask::UBTTask_AttackTask() : Super() {
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTTask_AttackTask::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
	Super::ExecuteTask(ownerComp, nodeMemory);

	AAIController* controller = ownerComp.GetAIOwner();
	if(controller) {
		UBlackboardComponent* bb = controller->GetBlackboardComponent();
		if(bb) {
			ABaseEnemyCharacter* bec = Cast<ABaseEnemyCharacter>(controller->GetCharacter());
			AAtlantisCharacter* ac = Cast<AAtlantisCharacter>(bb->GetValueAsObject(ENEMY_KEY));
			
			ac->UpdateHealth(-bec->damage);

			FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	FinishLatentTask(ownerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}