// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "TargetInRange.generated.h"

UCLASS()
class ATLANTIS_API UTargetInRange : public UBTDecorator_Blackboard {
	GENERATED_BODY()
public:
	UTargetInRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
};
