// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackTask.generated.h"

UCLASS()
class ATLANTIS_API UBTTask_AttackTask : public UBTTaskNode {
	GENERATED_BODY()
public:
	UBTTask_AttackTask();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};
