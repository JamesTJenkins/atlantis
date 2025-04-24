// Copyright James Jenkins. All Rights Reserved.

#include "Decorators/TargetInRange.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemyController.h"
#include "GameFramework/Character.h"

UTargetInRange::UTargetInRange() {
	NodeName = "Target In Range";
}

bool UTargetInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool valid = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent();
	if(valid && bb) {
		AActor* target = Cast<AActor>(bb->GetValueAsObject(BlackboardKey.SelectedKeyName));
		ABaseEnemyController* controller = Cast<ABaseEnemyController>(OwnerComp.GetAIOwner());
		ACharacter* aiCharacter = OwnerComp.GetAIOwner()->GetCharacter();

		if(aiCharacter && target && controller && FVector::Distance(aiCharacter->GetActorLocation(), target->GetActorLocation()) < controller->attackRange) {
			return true;
		}
	}
	
	return false;
}

FString UTargetInRange::GetStaticDescription() const {
	return FString("Target In Range Decorator");
}
