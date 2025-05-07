// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "Carriable.generated.h"

class UStaticMeshComponent;

UCLASS()
class ATLANTIS_API ACarriable : public ABaseInteractable {
	GENERATED_BODY()
public:
	ACarriable();

	virtual void OnInteract(AAtlantisCharacter* playerCharacter) override;

	void EnableDampening(bool enabled);
	void AddForce(FVector force);
private:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* staticMesh;

	float originalLinearDampening;
	float originalAngularDampening;
};
