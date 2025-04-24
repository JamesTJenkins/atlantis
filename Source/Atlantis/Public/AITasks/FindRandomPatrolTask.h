// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindRandomPatrolTask.generated.h"

UCLASS()
class ATLANTIS_API UBTTask_FindRandomPatrolTask : public UBTTaskNode {
	GENERATED_BODY()
public:
	UBTTask_FindRandomPatrolTask();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};
